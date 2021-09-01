/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_philos_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharvet <tcharvet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 14:00:12 by tcharvet          #+#    #+#             */
/*   Updated: 2021/09/01 16:32:47 by tcharvet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_philosophers_bonus.h"


void	fill_philo(t_data *data, t_philo *philos, int i)
{	
	memset(&philos[i], 0, sizeof(t_philo));
	philos[i].id = i + 1;
	philos[i].sleep_time = data->sleep_time * 1000;
	philos[i].meal_time = data->meal_time * 1000;
	philos[i].forks = data->forks;
	philos[i].screen = data->screen;
	philos[i].min_of_meal = data->min_of_meal;
	philos[i].error_code = &data->error_code;
	philos[i].active = &data->active;
	philos[i].begin_time = &data->begin_time;
	philos[i].begin_or_not = &data->begin_or_not;
}

int	create_and_launch_philos(t_data *data, t_philo *philos)
{
	int	i;

	i = -1;
	while (++i < data->philos_len)
	{
		philos[i].process = fork();
		if(philos[i].process == - 1)
			return (1);
		if (!philos[i].process)
			philo_routine(&philos[i]);
	}
	return (0);
}
