/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_philos.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharvet <tcharvet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 14:00:12 by tcharvet          #+#    #+#             */
/*   Updated: 2021/08/31 14:41:13 by tcharvet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

void	mutex_check(int (*func)(pthread_mutex_t *),
	pthread_mutex_t *mutex, int *error_code)
{
	if (func(mutex))
		*error_code = 1;
}

void	fill_philo(t_data *data, t_philo *philos, int i)
{	
	memset(&philos[i], 0, sizeof(t_philo));
	philos[i].id = i + 1;
	philos[i].sleep_time = data->sleep_time * 1000;
	philos[i].meal_time = data->meal_time * 1000;
	if (data->philos_len == 1)
	{
		philos[i].fork_one = &data->forks[i];
		philos[i].fork_two = &data->forks[i];
	}
	else
	{
		philos[i].fork_two = &data->forks[i];
		if (!i)
			philos[i].fork_one = &data->forks[data->philos_len - 1];
		else
			philos[i].fork_one = &data->forks[i - 1];
	}
	philos[i].screen = data->screen;
	philos[i].min_of_meal = data->min_of_meal;
	philos[i].error_code = &data->error_code;
	philos[i].active = &data->active;
	philos[i].begin_time = &data->begin_time;
	philos[i].begin_or_not = &data->begin_or_not;
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
		fill_philo(data, philos, i);
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
