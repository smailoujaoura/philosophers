#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <stdint.h>

long get_elapsed_time_microseconds(struct timeval start, struct timeval end)
{
    return (end.tv_sec - start.tv_sec) * 1000000L + (end.tv_usec - start.tv_usec);
}

void precise_usleep(long usec) {
    struct timeval start, current;
    long elapsed;
    long rem;

    gettimeofday(&start, NULL);
    do {
        gettimeofday(&current, NULL);
        elapsed = get_elapsed_time_microseconds(start, current);
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
		precise_usleep(500);
		timespec_get(&end, TIME_UTC);

		diff_ns = (end.tv_sec - start.tv_sec) * 1000000000L + 
				(end.tv_nsec - start.tv_nsec);

		diff_us = diff_ns / 1000;
		if (diff_us > 550 || diff_us < 400)
			break ;
		i++;
	}
	printf("i: %zu [%ld]\n", i, diff_us);
	return 0;
}

