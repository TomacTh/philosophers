/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_and_init.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharvet <tcharvet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 13:51:01 by tcharvet          #+#    #+#             */
/*   Updated: 2021/08/31 14:44:43 by tcharvet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

unsigned int	ft_atoi_verif(const char *str)
{
	ssize_t	num;

	num = 0;
	while ((*(char *)str >= '\t' && *(char *)str <= '\r')
		|| *(char *)str == ' ')
		str++;
	if (*(char *)str == '+')
		str++;
	if (!*str)
		return (-1);
	while (*(char *)str >= '0' && *(char *)str <= '9')
	{
		num = num * 10 + (*(char *)str - '0');
		str++;
	}
	if (num > UINT_MAX || num < 0 || *str)
		return (-1);
	return (num);
}

int	parse_args(int ac, char **av, t_data *data)
{
	int	bad_values;
	int	null_values;

	data->philos_len = ft_atoi_verif (av[1]);
	data->die_time = ft_atoi_verif (av[2]);
	data->meal_time = ft_atoi_verif (av[3]);
	data->sleep_time = ft_atoi_verif (av[4]);
	bad_values = data->philos_len < 0 || data->die_time < 0
		|| data->meal_time < 0 || data->sleep_time < 0;
	if (bad_values)
		return (ft_return_error(1, "Parsing error\n"));
	null_values = !data->philos_len || !data->die_time || !data->meal_time
		|| !data->sleep_time;
	if (null_values)
		return (0);
	if (ac == 6)
	{
		data->min_of_meal = ft_atoi_verif (av[5]);
		if (data->min_of_meal < 0)
			return (ft_return_error(1, "Parsing error\n"));
		if (!data->min_of_meal)
			return (0);
	}
	return (2);
}

int	init_mutex(pthread_mutex_t *forks, pthread_mutex_t *screen,
	int len, t_philo *philos)
{
	int	i;

	i = 0;
	if (pthread_mutex_init(screen, 0))
	{
		free(philos);
		free(forks);
		return (ft_return_error(1, "Cannot init mutex\n"));
	}
	while (i < len)
	{
		if (pthread_mutex_init(&forks[i], 0))
		{
			free(philos);
			destroy_mutex(screen, 1);
			destroy_mutex(forks, i);
			return (ft_return_error(1, "Cannot init mutex\n"));
		}
		++i;
	}
	return (0);
}

int	alloc_struct(t_data *data, pthread_mutex_t *screen)
{	
	data->screen = screen;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philos_len);
	if (!data->forks)
		return (ft_return_error(1, "Malloc error\n"));
	data->philos = malloc(sizeof(t_philo) * data->philos_len);
	if (!data->philos)
	{
		free(data->forks);
		return (ft_return_error(1, "Malloc error\n"));
	}
	return (0);
}

int	alloc_struct_and_init_mutexs(t_data *data, pthread_mutex_t *screen)
{
	if (alloc_struct(data, screen))
		return (1);
	data->active = 1;
	data->die_time_ms = data->die_time * 1000;
	if (!data->min_of_meal)
		data->min_of_meal = -1;
	if (init_mutex(data->forks, screen, data->philos_len, data->philos))
		return (1);
	return (0);
}
