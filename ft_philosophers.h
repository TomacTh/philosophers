/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharvet <tcharvet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 15:31:16 by tcharvet          #+#    #+#             */
/*   Updated: 2021/08/29 21:12:27 by tcharvet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PHILOSOPHERS_H
# define FT_PHILOSOPHERS_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <string.h>
# include <limits.h>
# include <sys/time.h>

typedef struct	s_philo
{
	unsigned int		id;
	useconds_t			sleep_time;
	useconds_t			meal_time;
	pthread_t			thread;
	pthread_mutex_t		*fork_one;
	pthread_mutex_t		*fork_two;
	pthread_mutex_t		*screen;
	size_t				*begin_time;
	size_t				last_meal;
	int					*begin_or_not;
	struct timeval 		current_time;
	unsigned int		num_of_meal;
	unsigned int		min_of_meal;
	int					*active;
	int					*error_code;
}						t_philo;

typedef struct s_data
{
	unsigned int		philos_len;
	unsigned int		die_time;
	unsigned int		meal_time;
	unsigned int		sleep_time;
	unsigned int		min_of_meal;
	int					active;
	int					error_code;
	struct timeval 		time;
	int					begin_or_not;
	size_t				begin_time;
	t_philo				*philos;
	pthread_mutex_t		*forks;
	pthread_mutex_t		*screen;
}						t_data;
size_t	convert_in_ms(struct timeval current_time);

#endif
