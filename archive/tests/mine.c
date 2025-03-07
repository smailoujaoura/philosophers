#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <stdint.h>

void precise_sleep(size_t micro_secs)
{
	size_t			remainder;
	size_t			elapsed;
	struct timeval	current;
	struct timeval	start;

	gettimeofday(&start, NULL);
	elapsed = 0;
	while (elapsed < micro_secs)
	{
		gettimeofday(&current, NULL);
		elapsed = (current.tv_sec - start.tv_sec) * 1000000 + (current.tv_usec - start.tv_usec);
		remainder = micro_secs - elapsed;

		if (remainder > 500) 
			usleep(remainder / 2);
	}
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
		if (diff_us > 505 || diff_us < 499)
			break ;
		i++;
	}
	printf("i: %zu [%ld]\n", i, diff_us);
	return 0;
}

