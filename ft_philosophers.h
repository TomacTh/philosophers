/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharvet <tcharvet@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 15:31:16 by tcharvet          #+#    #+#             */
/*   Updated: 2021/08/28 22:19:19 by tcharvet         ###   ########.fr       */
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
	int					id;
	int					times[3];
	pthread_t			thread;
	pthread_mutex_t		*fork_one;
	pthread_mutex_t		*fork_two;
	pthread_mutex_t		*screen;
	int					last_meal;
	int					num_of_meal;
	int					min_of_meal;
	int					active;
	int					error_code;
}						t_philo;

typedef struct s_data
{
	int					philos_len;
	int					die_time;
	int					meal_time;
	int					sleep_time;
	int					min_of_meal;
	int					*active;
	int					*error_code;
	t_philo				*philos;
	pthread_mutex_t		*forks;
	pthread_mutex_t		*screen;
}						t_data;

#endif
