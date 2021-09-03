/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_monitoring_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharvet <tcharvet@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 14:05:25 by tcharvet          #+#    #+#             */
/*   Updated: 2021/09/03 15:37:56 by tcharvet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

void	init_begin_time(t_data *data)
{
	struct timeval	time;

	gettimeofday(&time, 0);
	data->begin_time = convert_in_ms(time);
	data->begin_or_not = 1;
}

void	print_die(t_philo *philo, useconds_t die_time)
{
	size_t			time_size;

	sem_wait(philo->screen);
	if (!*philo->error_code)
	{
		time_size = philo->last_meal + die_time;
		printf("%lu %i died\n", time_size, philo->id);
	}
}

int	check_die(t_data *data, t_philo *philo, int is_die,
	int philo_finish)
{
	if (is_die && !philo_finish)
	{
		data->active = 0;
		print_die(philo, data->die_time);
		return (1);
	}
	return (0);
}

void	process_philo(t_philo *philo, t_data *data)
{
	size_t	size;
	int		is_die;
	int		philo_finish;

	if (pthread_create(&philo->thread, 0, philo_routine, philo))
		exit(2);
	while (data->active)
	{
		gettimeofday(&data->time, 0);
		size = (convert_in_ms(data->time) - data->begin_time);
		is_die = (size - philo->last_meal) >= data->die_time;
		philo_finish = data->min_of_meal == philo->num_of_meal;
		if (check_die(data, philo, is_die, philo_finish))
			exit(1);
		else if (philo_finish)
		{
			pthread_join(philo->thread, 0);
			exit (0);
		}
		usleep(1000);
	}
	exit(0);
}
