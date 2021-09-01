/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers_bonus.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharvet <tcharvet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 15:31:16 by tcharvet          #+#    #+#             */
/*   Updated: 2021/09/01 16:58:45 by tcharvet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PHILOSOPHERS_bonus_H
# define FT_PHILOSOPHERS_bonus_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>
# include <limits.h>
# include <sys/time.h>
# include <semaphore.h>
# include <signal.h>

typedef struct s_philo
{
	unsigned int		id;
	useconds_t			sleep_time;
	useconds_t			meal_time;
	pid_t				process;
	sem_t				*screen;
	sem_t				*forks;
	size_t				*begin_time;
	size_t				last_meal;
	int					*begin_or_not;
	int					num_of_meal;
	int					min_of_meal;
	int					*active;
	int					*error_code;
}						t_philo;

typedef struct s_data
{
	int					philos_len;
	unsigned int		die_time;
	unsigned int		meal_time;
	unsigned int		sleep_time;
	int					min_of_meal;
	useconds_t			die_time_ms;
	int					philos_finish;
	int					active;
	int					error_code;
	struct timeval		time;
	int					begin_or_not;
	size_t				begin_time;
	t_philo				*philos;
	sem_t				*screen;
	sem_t				*forks;
}						t_data;

enum e_state {eating, sleeping, forking, thinking};

void	*philo_routine(t_philo *philo);
int		create_and_launch_philos(t_data *data, t_philo *philos);
void	*supervisor_routine(t_data *data);

void	sleep_if_no_error(useconds_t time, int *error_code, int *active);
void	destroy_mutex(pthread_mutex_t *mutex, int len);
int		ft_return_error(int code, char *str);
size_t	convert_in_ms(struct timeval current_time);
void	mutex_check(int (*func)(pthread_mutex_t *),
			pthread_mutex_t *mutex, int *error_code);
void	ft_putstr_fd(char *s, int fd);

int		alloc_struct(t_data *data, pthread_mutex_t *screen);
int		alloc_struct_and_init_mutexs(t_data *data, pthread_mutex_t *screen);
int		parse_args(int ac, char **av, t_data *data);
int		alloc_struct(t_data *data, pthread_mutex_t *screen);

#endif
