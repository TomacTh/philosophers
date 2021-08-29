

#include "ft_philosophers.h"


void	*philo_routine(void *addr)
{
	pthread_mutex_t *mutex;

	mutex = (pthread_mutex_t *)addr;
	printf("first lock int %i\n",pthread_mutex_lock(mutex));
	printf("yes enter in first lock\n");
	//pthread_mutex_lock(mutex);
	//printf("first lock int %i\n",pthread_mutex_lock(mutex));
	printf("first lock int %i\n",pthread_mutex_unlock(mutex));
	pthread_mutex_unlock(mutex);
	return (NULL);

}

int main(void)
{
	pthread_t thread;
	pthread_mutex_t mutex;
	struct timeval time;




	//pthread_mutex_init(&mutex, 0);
	//pthread_create(&thread, 0, philo_routine, &mutex);
	//pthread_join(thread, 0);
	time.tv_sec = 0;
	time.tv_usec = 0;
	
	gettimeofday(&time, 0);
	printf("%ld\n%ui\n", time.tv_sec, time.tv_usec);

	return (0);



}

