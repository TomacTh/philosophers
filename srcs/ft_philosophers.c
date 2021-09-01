/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharvet <tcharvet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 09:42:19 by tcharvet          #+#    #+#             */
/*   Updated: 2021/09/01 14:12:26 by tcharvet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

int	free_join_and_quit(t_data *data, char *str)
{	
	int	i;

	if (str)
		ft_putstr_fd(str, 2);
	i = -1;
	if (data->philos_len > 1)
	{
		while (++i < data->philos_len)
		{	
			if (pthread_join(data->philos[i].thread, 0))
				ft_putstr_fd("Error when pthread_join\n", 2);
		}
	}
	free(data->philos);
	free(data->forks);
	return (data->error_code);
}

void	manage_status(enum e_state philo_e_state, t_philo *philo)
{	
	struct timeval	time;
	size_t			time_size;
	static char		*status[] = {"is eating",
		"is sleeping", "has taken a fork", "is thinking"};

	mutex_check(pthread_mutex_lock, philo->screen, philo->error_code);
	if (*philo->active && !(*philo->error_code))
	{
		gettimeofday(&time, 0);
		time_size = convert_in_ms(time);
		time_size -= *philo->begin_time;
		if (philo_e_state == eating)
		{
			philo->last_meal = time_size;
			++(philo->num_of_meal);
		}
		printf("%lu %i %s\n", time_size, philo->id, status[philo_e_state]);
	}
	mutex_check(pthread_mutex_unlock, philo->screen, philo->error_code);
}

void	init_begin_time(t_philo *philo)
{	
	struct timeval	time;

	gettimeofday(&time, 0);
	*philo->begin_time = convert_in_ms(time);
	*philo->begin_or_not = 1;
}

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (!(*philo->begin_or_not))
		init_begin_time(philo);
	while (*philo->active && !(*philo->error_code))
	{
		manage_status(thinking, philo);
		mutex_check(pthread_mutex_lock, philo->fork_one, philo->error_code);
		manage_status(forking, philo);
		mutex_check(pthread_mutex_lock, philo->fork_two, philo->error_code);
		manage_status(forking, philo);
		manage_status(eating, philo);
		sleep_if_no_error(philo->meal_time, philo->error_code, philo->active);
		mutex_check(pthread_mutex_unlock, philo->fork_two, philo->error_code);
		mutex_check(pthread_mutex_unlock, philo->fork_one, philo->error_code);
		if (philo->num_of_meal == philo->min_of_meal)
			return (NULL);
		manage_status(sleeping, philo);
		sleep_if_no_error(philo->sleep_time, philo->error_code, philo->active);
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	t_data			data;
	pthread_mutex_t	screen;
	int				res;

	if (ac < 5 || ac > 6)
		return (ft_return_error(1, "Wrong number of arguments\n"));
	memset(&data, 0, sizeof(t_data));
	res = parse_args(ac, av, &data);
	if (res < 2)
		return (res);
	if (alloc_struct_and_init_mutexs(&data, &screen))
		return (1);
	if (pthread_create(&data.supervisor, 0, supervisor_routine, &data))
		return (free_join_and_quit(&data, "pthread_create Error\n"));
	if (create_and_launch_philos(&data, data.philos))
		return (free_join_and_quit(&data, "pthread_create Error\n"));
	if (pthread_join(data.supervisor, 0))
		return (free_join_and_quit(&data, "pthread_join Error\n"));
	if (data.error_code)
		return (free_join_and_quit(&data, "Thread error\n"));
	return (free_join_and_quit(&data, 0));
}
