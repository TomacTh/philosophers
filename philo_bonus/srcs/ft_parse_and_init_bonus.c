/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_and_init_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharvet <tcharvet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 13:51:01 by tcharvet          #+#    #+#             */
/*   Updated: 2021/09/01 15:52:15 by tcharvet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

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
