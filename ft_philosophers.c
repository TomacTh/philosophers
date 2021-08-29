/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharvet <tcharvet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 21:37:56 by tcharvet          #+#    #+#             */
/*   Updated: 2021/08/29 16:28:22 by tcharvet         ###   ########.fr       */
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
		return (ft_return_error(1, "Arguments must be positive unsigned integer\n"));
	null_values = !data->philos_len || !data->die_time || !data->meal_time
		|| !data->sleep_time;
	if (null_values)
		return (0);
	if (ac == 6)
	{
		data->min_of_meal = ft_atoi_verif(av[5]);
		if (data->min_of_meal < 0)
			return (ft_return_error(1, "Arguments must be positive unsigned integer\n"));
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

void	destroy_and_free(t_data *data)
{
	destroy_mutex(data->screen, 1);
	destroy_mutex(data->forks, data->philos_len);
	free(data->philos);
	free(data->forks);
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
	return (0);
}

int	alloc_struct_and_init_mutexs(t_data *data, pthread_mutex_t *screen)
{
	if (alloc_struct(data, screen))
		return (1);
	data->active = 1;
	if (init_mutex(data->forks, screen, data->philos_len, data->philos))
		return (1);
	return (0);
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

void	manage_status(int index_status, t_philo *philo)
{
	size_t time;

	static	char *status[] = {"has taken a fork", "is eating", "is sleeping", "is thinking"};
	pthread_mutex_lock(philo->screen);
	if (gettimeofday(&philo->current_time, 0) == -1)
		*philo->error_code = 1;	
	if (*philo->active && !(*philo->error_code))
		time = convert_in_ms(philo->current_time);
		time -= *philo->begin_time;
		if(index_status == 1)
			philo->last_meal = time;
		printf("%lu %i %s\n", time, philo->id, status[index_status]);
	pthread_mutex_unlock(philo->screen);
}

void	init_begin_time(t_philo *philo)
{
	if (gettimeofday(&philo->current_time, 0) < 0)
		*philo->error_code = 1;	
	else
		*philo->begin_time = convert_in_ms(philo->current_time);
	*philo->begin_or_not = 1;
}

void	mutex_check_error(int(*func)(pthread_mutex_t *), pthread_mutex_t *mutex, int *error_code)
{
	if (func(mutex) != 0)
		*error_code = 1;
}

void	*philo_routine(void *data)
{
	t_philo *philo;

	philo = (t_philo *)data;
	if (!(*philo->begin_or_not))
		init_begin_time(philo);
	while(*philo->active && !(*philo->error_code))
	{
		manage_status(3, philo);
		pthread_mutex_lock(philo->fork_one);
		manage_status(0, philo);
		pthread_mutex_lock(philo->fork_two);
		manage_status(0, philo);
		manage_status(1, philo);
		usleep(philo->meal_time);
		++(philo->num_of_meal);
		pthread_mutex_unlock(philo->fork_two);
		pthread_mutex_unlock(philo->fork_one);
		manage_status(2, philo);
		usleep(philo->sleep_time);
	}
	return (0);
}

size_t	convert_in_ms(struct timeval current_time)
{
	size_t	res;

	res = current_time.tv_usec/1000 + (current_time.tv_sec * 1000);
	return (res);
}

void	create_and_launch_philos(t_data *data, t_philo *philos)
{
	int	i;

	i = -1;
	while (++i < data->philos_len)
		fill_philo(data, philos, i);
	i = -1;
	while (++i < data->philos_len)
	{
		if (!(philos[i].id % 2))
		{
			pthread_create(&philos[i].thread, 0, philo_routine, &philos[i]);
			pthread_detach(philos[i].thread);
		}
	}
	if (data->philos_len > 1)
		usleep(data->meal_time / 2);
	i = -1;
	while (++i < data->philos_len)
	{
		if (philos[i].id % 2)
		{
			pthread_create(&philos[i].thread, 0, philo_routine, &philos[i]);
			pthread_detach(philos[i].thread);
		}
	}
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
	if (alloc_struct_and_init_mutexs(&data, &screen))
		return (1);
	create_and_launch_philos(&data, data.philos);
	int		i;
	size_t	size;
	
	
	while (data.active && !data.error_code)
	{
		i = -1;
		while(++i < data.philos_len)
		{
			if( (gettimeofday(&data.time, 0) == -1)
				data.error_code = 1;
			if (convert_in_ms(data.time) - )

		}
		
	}
}
