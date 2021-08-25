/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharvet <tcharvet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 21:37:56 by tcharvet          #+#    #+#             */
/*   Updated: 2021/08/25 23:06:59 by tcharvet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (s)
		write(fd, s, ft_strlen(s));
}

int	ft_atoi_verif(const char *str)
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
	if (num > INT_MAX || num < 0 || *str)
		return (-1);
	return (num);
}

void	*print_thread(void *data)
{	
	void **tab = (void**)data;

	pthread_mutex_lock(tab[1]);

	printf("We are in thread %i\n", * (int *)tab[0]);
	pthread_mutex_unlock(tab[1]);
	return (NULL);

}

int	ft_return_error(int code, char *str)
{	
	if(*str)
		ft_putstr_fd(str, 2);
	return (code);
}

int	parse_args(int ac, char **av, t_data *data)
{	
	int	bad_values;
	int	null_values;

	data->philos_len = ft_atoi_verif(av[1]);
	data->die_time = ft_atoi_verif(av[2]);
	data->meal_time = ft_atoi_verif(av[3]);
	data->sleep_time = ft_atoi_verif(av[4]);
	bad_values = data->philos_len < 0 || data->die_time < 0|| data->meal_time < 0
	|| data->sleep_time < 0;
	if (bad_values)
		return (ft_return_error(1, "Arguments must be positive integer\n"));
	null_values = !data->philos_len || !data->die_time || !data->meal_time 
		|| !data->sleep_time;
	if (null_values)
		return (0);
	if (ac == 6)
	{
		data->min_of_meal = ft_atoi_verif(av[5]);
		if (data->min_of_meal < 0)
			return (ft_return_error(1, "Arguments must be positive integer\n"));
		if (!data->min_of_meal)
			return (0);
	}
	return (2);
}

void	destroy_mutex(pthread_mutex_t *mutex, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if(pthread_mutex_destroy(&mutex[i]))
			ft_putstr_fd("Error when destroy mutex\n", 2);
		++i;
	}
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

int	init_mutex(pthread_mutex_t *forks, pthread_mutex_t *screen, int len, t_philo *philos)
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
	return (1);
}

int	init_struct(t_data *data, pthread_mutex_t *screen)
{	
	if (alloc_struct(data, screen))
		return (1);
	if (init_mutex(data->forks, screen, data->philos_len, data->philos))
		return (1);
	if (	
}

int main(int ac, char **av)
{
	t_data	data;
	pthread_mutex_t	screen;
	int		res;

	if(ac < 5 || ac > 6)
		return (ft_return_error(1, "Wrong number of arguments\n"));
	memset(&data, 0, sizeof(t_data));
	res = parse_args(ac, av, &data);
	if (res < 2)
		return (res);

}
