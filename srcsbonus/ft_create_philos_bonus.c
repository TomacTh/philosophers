/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_philos_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharvet <tcharvet@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 14:00:12 by tcharvet          #+#    #+#             */
/*   Updated: 2021/09/03 15:50:20 by tcharvet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

void	fill_philo(t_data *data, t_philo *philo, int i)
{
	memset(philo, 0, sizeof(t_philo));
	philo->id = i + 1;
	philo->sleep_time = data->sleep_time * 1000;
	philo->meal_time = data->meal_time * 1000;
	philo->forks = data->forks;
	philo->screen = data->screen;
	philo->min_of_meal = data->min_of_meal;
	philo->error_code = &data->error_code;
	philo->active = &data->active;
	philo->begin_time = &data->begin_time;
	philo->begin_or_not = &data->begin_or_not;
}

int	create_and_launch_philos(t_data *data, t_philo *philos)
{
	int	i;

	i = -1;
	while (++i < data->philos_len)
		fill_philo(data, &data->philos[i], i);
	i = -1;
	init_begin_time(data);
	while (++i < data->philos_len)
	{
		philos[i].process = fork();
		if (philos[i].process == -1)
			return (1);
		if (philos[i].process == 0)
			process_philo(&philos[i], data);
	}
	return (0);
}
