/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_and_init_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharvet <tcharvet@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 13:51:01 by tcharvet          #+#    #+#             */
/*   Updated: 2021/09/03 15:18:34 by tcharvet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

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

void	init_sem(sem_t **sem, char *str, int value)
{
	*sem = sem_open(str, O_CREAT | O_EXCL, 0644, value);
	if (*sem == SEM_FAILED)
	{
		sem_unlink(str);
		*sem = sem_open(str, O_CREAT | O_EXCL, 0644, value);
	}
}

int	init_semaphore_and_alloc_philos(t_data *data, sem_t **screen, sem_t **forks)
{
	data->active = 1;
	data->die_time_ms = data->die_time * 1000;
	if (!data->min_of_meal)
		data->min_of_meal = -1;
	init_sem(screen, "screen", 1);
	init_sem(forks, "forks", data->philos_len);
	data->forks = *forks;
	data->screen = *screen;
	if (data->forks == SEM_FAILED || data->screen == SEM_FAILED)
		return (free_kill_and_quit(data, "Error when init semaphore\n"));
	if (alloc_philos(data))
		return (free_kill_and_quit(data, "Malloc error\n"));
	return (0);
}

unsigned int	ft_atoi_verif(const char *str, int *error_code)
{
	ssize_t	num;

	num = 0;
	while ((*(char *)str >= '\t' && *(char *)str <= '\r')
		|| *(char *)str == ' ')
		str++;
	if (*(char *)str == '+')
		str++;
	if (!*str)
	{
		*error_code = 1;
		return (1);
	}
	while (*(char *)str >= '0' && *(char *)str <= '9')
	{
		num = num * 10 + (*(char *)str - '0');
		str++;
	}
	if (num > UINT_MAX || num < 0 || *str)
		*error_code = 1;
	return (num);
}

int	parse_args(int ac, char **av, t_data *data)
{
	int	null_values;

	data->philos_len = ft_atoi_verif (av[1], &data->error_code);
	data->die_time = ft_atoi_verif (av[2], &data->error_code);
	data->meal_time = ft_atoi_verif (av[3], &data->error_code);
	data->sleep_time = ft_atoi_verif (av[4], &data->error_code);
	if (data->error_code)
		return (ft_return_error(1, "Parsing error\n"));
	null_values = !data->philos_len || !data->die_time || !data->meal_time
		|| !data->sleep_time;
	if (null_values)
		return (0);
	if (ac == 6)
	{
		data->min_of_meal = ft_atoi_verif (av[5], &data->error_code);
		if (data->error_code)
			return (ft_return_error(1, "Parsing error\n"));
		if (!data->min_of_meal)
			return (0);
	}
	return (2);
}
