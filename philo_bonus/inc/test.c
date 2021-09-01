/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharvet <tcharvet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 15:54:57 by tcharvet          #+#    #+#             */
/*   Updated: 2021/09/01 16:06:22 by tcharvet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"
#include <semaphore.h>
#include <sys/stat.h>
#include <errno.h>

int	init_sem(sem_t *sem, int len)
{	
	sem = sem_open("forks", O_CREAT | O_EXCL ,S_IRWXU, len);
	if (sem == SEM_FAILED)
		sem_unlink("forks");
	sem = sem_open("forks", O_CREAT | O_EXCL ,S_IRWXU, len);
	if (sem == SEM_FAILED)
		return (1)
	return (0);
}	

int	main (void)
{
	sem_t sem;

	sem_func(&sem);
	
	return (0);
}
