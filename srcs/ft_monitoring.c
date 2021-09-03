/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_monitoring.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharvet <tcharvet@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 14:05:25 by tcharvet          #+#    #+#             */
/*   Updated: 2021/09/03 15:42:50 by tcharvet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

void	sleep_if_no_error(useconds_t time, int *error_code, int *active)
{
	if (*active && !(*error_code))
		usleep(time);
}

void	print_die(t_philo *philo, useconds_t die_time)
{
	size_t			time_size;

	mutex_check(pthread_mutex_lock, philo->screen, philo->error_code);
	if (!*philo->error_code)
	{
		time_size = philo->last_meal + die_time;
		printf("%lu %i died\n", time_size, philo->id);
	}
	mutex_check(pthread_mutex_unlock, philo->screen, philo->error_code);
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

int	check_eat_required(t_data *data, int philo_finish)
{
	if (data->min_of_meal != -1)
	{
		if (philo_finish)
			++data->philos_finish;
		if (data->philos_finish == data->philos_len)
		{
			data->active = 0;
			return (1);
		}
	}
	return (0);
}

void	*supervisor_routine(void *addr)
{
	t_data	*data;
	int		i;
	size_t	size;
	int		is_die;
	int		philo_finish;

	data = (t_data *)addr;
	while (data->active && !data->error_code)
	{
		i = -1;
		while (++i < data->philos_len && data->begin_or_not)
		{
			gettimeofday(&data->time, 0);
			size = (convert_in_ms(data->time) - data->begin_time);
			is_die = (size - data->philos[i].last_meal) >= data->die_time;
			philo_finish = data->min_of_meal == data->philos[i].num_of_meal;
			if (check_die(data, &data->philos[i], is_die, philo_finish))
				return (0);
			else if (check_eat_required(data, philo_finish))
				return (0);
		}
		usleep(1000);
	}
	return (0);
}
