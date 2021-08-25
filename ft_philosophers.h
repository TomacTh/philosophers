/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharvet <tcharvet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 15:31:16 by tcharvet          #+#    #+#             */
/*   Updated: 2021/08/25 19:54:12 by tcharvet         ###   ########.fr       */
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

typedef struct s_data
{
	int					philos_len;
	int					die_time;
	int					meal_time;
	int					sleep_time;
	int					min_of_meal;
	pthread_t			*philos;
	pthread_mutex_t		*forks;
	pthread_mutex_t		*screen;
}						t_data;


typedef struct	s_philo
{	
	int					id;
	int					times[3];
	pthread_t			thread;
	pthread_mutex_t		*forks;
	pthread_mutex_t		*screen;
	int					last_meal;
	int					num_of_meal;
	int					min_of_meal;
}						t_philo;

#endif
