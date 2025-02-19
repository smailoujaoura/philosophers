#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <stdint.h>

void precise_sleep(long usec)
{
	struct timeval start, current;
	long elapsed;
	long rem;

	elapsed = 0;
	gettimeofday(&start, NULL);
	do
	{
		gettimeofday(&current, NULL);
		elapsed = (current.tv_sec - start.tv_sec) * 1000000 + (current.tv_usec - start.tv_usec);
		rem = usec - elapsed;

		if (rem > 1000) 
			usleep(rem / 2);
	} while (elapsed < usec);
}


int main(void)
{
	struct timespec start;
	struct timespec end;
	int64_t diff_ns;
	int64_t diff_us;
	size_t	i;

	i = 0;
	while (1)
	{
		timespec_get(&start, TIME_UTC);
		precise_sleep(500);
		timespec_get(&end, TIME_UTC);

		diff_ns = (end.tv_sec - start.tv_sec) * 1000000000L + 
				(end.tv_nsec - start.tv_nsec);

		diff_us = diff_ns / 1000;
		if (diff_us > 505 || diff_us < 495)
			break ;
		i++;
	}
	printf("i: %zu [%ld]\n", i, diff_us);
	return 0;
}

	// printf("%ld\n", start.tv_sec);

	// while (1)
	// {
	// 	timespec_get(&start, TIME_UTC);
	// 	usleep(1000);
	// 	timespec_get(&end, TIME_UTC);
	// 	if ((end.tv_nsec - start.tv_nsec) / 1000 > 5000)
	// 		break ;
	// }

