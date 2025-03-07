#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

void *start(void *)
{
	printf("testing\n");
	sleep(3);
	printf("about to exit\n");
	return (NULL);
}

int main(void)
{
	pthread_t	id;

	pthread_create(&id, NULL, start, NULL);
	printf("created\n");
	// pthread_detach(id);
	printf("detached\n");
	// pthread_join(id, NULL);
}