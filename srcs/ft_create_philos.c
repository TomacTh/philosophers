/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_philos.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharvet <tcharvet@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 14:00:12 by tcharvet          #+#    #+#             */
/*   Updated: 2021/09/01 21:24:06 by tcharvet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

void	mutex_check(int (*func)(pthread_mutex_t *),
	pthread_mutex_t *mutex, int *error_code)
{
	if (func(mutex))
		*error_code = 1;
}

void	fill_philo(t_data *data, t_philo *philo, int i)
{
	memset(philo, 0, sizeof(t_philo));
	philo->id = i + 1;
	philo->sleep_time = data->sleep_time * 1000;
	philo->meal_time = data->meal_time * 1000;
	if (data->philos_len == 1)
	{
		philo->fork_one = &data->forks[i];
		philo->fork_two = &data->forks[i];
	}
	else
	{
		philo->fork_two = &data->forks[i];
		if (!i)
			philo->fork_one = &data->forks[data->philos_len - 1];
		else
			philo->fork_one = &data->forks[i - 1];
	}
	philo->screen = data->screen;
	philo->min_of_meal = data->min_of_meal;
	philo->error_code = &data->error_code;
	philo->active = &data->active;
	philo->begin_time = &data->begin_time;
	philo->begin_or_not = &data->begin_or_not;
}

int	launch_first_group_philos(t_data *data, t_philo *philos)
{
	int	i;

	i = -1;
	while (++i < data->philos_len)
	{
		if (!(philos[i].id % 2))
		{
			if (pthread_create(&philos[i].thread, 0, philo_routine, &philos[i]))
				return (1);
		}
	}
	return (0);
}

int	launch_second_group_philos(t_data *data, t_philo *philos)
{
	int	i;

	i = -1;
	while (++i < data->philos_len)
	{
		if (philos[i].id % 2)
		{
			if (pthread_create(&philos[i].thread, 0, philo_routine, &philos[i]))
				return (1);
		}
	}
	return (0);
}

int	create_and_launch_philos(t_data *data, t_philo *philos)
{
	int	i;

	i = -1;
	while (++i < data->philos_len)
		fill_philo(data, &philos[i], i);
	if (launch_first_group_philos(data, philos))
	{
		data->error_code = 1;
		return (1);
	}
	if (data->philos_len > 1)
		usleep(((data->meal_time * 1000)));
	if (launch_second_group_philos(data, philos))
	{
		data->error_code = 1;
		return (1);
	}
	return (0);
}
