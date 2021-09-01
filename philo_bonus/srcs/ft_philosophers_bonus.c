/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharvet <tcharvet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 09:42:19 by tcharvet          #+#    #+#             */
/*   Updated: 2021/09/01 17:54:33 by tcharvet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

void	close_sem(sem_t *sem)
{
	if (sem != SEM_FAILED)
		sem_close(sem);
}

int	free_kill_and_quit(t_data *data, char *str)
{	
	int	i;

	if (str)
		ft_putstr_fd(str, 2);
	i = -1;
	while (++i < data->philos_len)
		kill(data->philos[i].process, 9);
	close_sem(data->screen);
	close_sem(data->forks);
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

	sem_wait(philo->screen);
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
	sem_post(philo->screen);
}

void	init_begin_time(t_philo *philo)
{
	struct timeval	time;

	gettimeofday(&time, 0);
	*philo->begin_time = convert_in_ms(time);
	*philo->begin_or_not = 1;
}

void	*philo_routine(t_philo *philo)
{
	if (!(*philo->begin_or_not))
		init_begin_time(philo);
	while (*philo->active && !(*philo->error_code))
	{
		manage_status(thinking, philo);
		sem_wait(philo->forks);
		manage_status(forking, philo);
		sem_wait(philo->forks);
		manage_status(forking, philo);
		manage_status(eating, philo);
		sleep_if_no_error(philo->meal_time, philo->error_code, philo->active);
		sem_post(philo->forks);
		sem_post(philo->forks);
		if (philo->num_of_meal == philo->min_of_meal)
			return (NULL);
		manage_status(sleeping, philo);
		sleep_if_no_error(philo->sleep_time, philo->error_code, philo->active);
	}
	return (NULL);
}

int	alloc_philos(t_data *data)
{	
	data->philos = malloc(sizeof(t_philo) * data->philos_len);
	if (!data->philos)
	{
		free(data->forks);
		return (ft_return_error(1, "Malloc error\n"));
	}
	return (0);
}

int	init_sem(sem_t *sem, char *str, int value)
{
	sem = sem_open(str, O_CREAT | O_EXCL, S_IRWXU, value);
	if (sem == SEM_FAILED)
	{
		sem_unlink(str);
		sem = sem_open(str, O_CREAT | O_EXCL ,S_IRWXU, value);
		if (sem == SEM_FAILED)
			return (1);
	}
	return (0);
}

int	init_semaphore_and_alloc_philos(t_data *data, sem_t *screen, sem_t *forks)
{
	int	bool;

	data->active = 1;
	data->die_time_ms = data->die_time * 1000;
	if (!data->min_of_meal)
		data->min_of_meal = -1;
	bool = (init_sem(data->screen, "screen", 1)
		&& init_sem(data->forks, "forks", data->philos_len));
	data->screen = screen;
	data->forks = forks;
	if (bool)
		return (free_kill_and_quit(data, "Error when init semaphore\n"));
	if (alloc_philos(data))
		return (free_kill_and_quit(data, "Malloc error\n"));
	return (0);
}


int	main(int ac, char **av)
{
	t_data			data;
	sem_t			screen;
	sem_t			forks;
	int				res;

	if (ac < 5 || ac > 6)
		return (ft_return_error(1, "Wrong number of arguments\n"));
	memset(&data, 0, sizeof(t_data));
	res = parse_args(ac, av, &data);
	if (res < 2)
		return (res);
	if (init_semaphore_and_alloc_philos(&data, &forks, &screen))
		return (1);
	if (create_and_launch_philos(&data, data.philos))
		return (free_kill_and_quit(&data, "Proccess error\n"));
	supervisor_routine(&data);
	if (data.error_code)
		return (free_kill_and_quit(&data, "Thread error\n"));
	return (free_kill_and_quit(&data, 0));
}
