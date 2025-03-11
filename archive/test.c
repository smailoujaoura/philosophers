# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <semaphore.h>
# include <string.h>
# include <pthread.h>
# include <stdint.h>
# include <sys/time.h>
# include <sys/wait.h>

int main(void)
{
	sem_t *sem = sem_open("/soujaour_philo", O_CREAT | O_EXCL, 0700, 1);
	if (sem == SEM_FAILED)
		printf("failed\n");
	else
		printf("Succeded\n");
	// sem_unlink("/soujaour_philo");
	while (1)
	{
		if (-1 == sem_wait(sem))
			printf("Could not wait on sem\n");
		else
			printf("wait success on sem\n");
		printf("%ld\n", sem->__align);
		printf("%s\n", sem->__size);
		sem_post(sem);
		sleep(1);
	}
}

❯ ./a.out
failed
[1]    262819 segmentation fault (core dumped)  ./a.out


when you don't link a semaphore and sem_open fails and procceed to use what sem_t supposedly has 
so always check the return and protect accordingly