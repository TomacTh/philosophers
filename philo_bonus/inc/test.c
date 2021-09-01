/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharvet <tcharvet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 15:54:57 by tcharvet          #+#    #+#             */
/*   Updated: 2021/09/01 18:10:02 by tcharvet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <semaphore.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>

int	init_sem(sem_t *sem, int len)
{	
	sem = sem_open("forks", O_CREAT | O_EXCL , len);
	if (sem == SEM_FAILED)
		sem_unlink("forks");
	sem = sem_open("forks", O_CREAT | O_EXCL , len);
	if (sem == SEM_FAILED)
		return (1);
	return (0);
}	

int	main (void)
{
	sem_t sem;

	init_sem(&sem, 1);
	printf("before lock\n");
	sem_wait(&sem);
	printf("before lock\n");
	sem_wait(&sem);
	printf("before lock\n");
	sem_post(&sem);
	
	return (0);
}
