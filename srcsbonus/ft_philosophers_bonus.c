/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharvet <tcharvet@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 09:42:19 by tcharvet          #+#    #+#             */
/*   Updated: 2021/09/03 15:49:50 by tcharvet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

int	free_kill_and_quit(t_data *data, char *str)
{
	int	i;

	if (str)
		ft_putstr_fd(str, 2);
	i = -1;
	while (++i < data->philos_len)
		kill(data->philos[i].process, 9);
	sem_close(data->screen);
	sem_close(data->forks);
	free(data->philos);
	exit(data->error_code);
}

void	manage_status(enum e_state philo_e_state, t_philo *philo)
{
	struct timeval	time;
	size_t			time_size;
	static char		*status[] = {"is eating",
		"is sleeping", "has taken a fork", "is thinking"};

	sem_wait(philo->screen);
	if (*philo->active)
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
	sem_post(philo->screen);
}

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	while (*philo->active)
	{
		manage_status(thinking, philo);
		sem_wait(philo->forks);
		manage_status(forking, philo);
		sem_wait(philo->forks);
		manage_status(forking, philo);
		manage_status(eating, philo);
		sleep_if_active(philo->meal_time, philo->active);
		sem_post(philo->forks);
		sem_post(philo->forks);
		if (philo->num_of_meal == philo->min_of_meal)
			return (0);
		manage_status(sleeping, philo);
		sleep_if_active(philo->sleep_time + 1000, philo->active);
	}
	return (0);
}

int	handle_process_philo(t_data *data)
{
	int	status;
	int	res;

	res = 0;
	while (waitpid(-1, &status, 0) != -1)
	{
		res = WEXITSTATUS(status);
		if (res == 1)
			return (free_kill_and_quit(data, 0));
		else if (res == 2)
			return (free_kill_and_quit(data, "Error when creating thread"));
	}
	return (free_kill_and_quit(data, 0));
}

int	main(int ac, char **av)
{
	t_data			data;
	sem_t			*screen;
	sem_t			*forks;
	int				res;

	if (ac < 5 || ac > 6)
		return (ft_return_error(1, "Wrong number of arguments\n"));
	memset(&data, 0, sizeof(t_data));
	res = parse_args(ac, av, &data);
	if (res < 2)
		return (res);
	if (init_semaphore_and_alloc_philos(&data, &screen, &forks))
		return (1);
	if (create_and_launch_philos(&data, data.philos))
		return (free_kill_and_quit(&data, "Proccess error\n"));
	return (handle_process_philo(&data));
}
