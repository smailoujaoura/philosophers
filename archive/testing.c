#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <stdint.h>

ssize_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void    ft_usleep(size_t millisec)
{
    size_t    starttime;

    starttime = get_time();
    while ((get_time() - starttime) < millisec)
        usleep(500);
}

int main(void)
{
	struct timespec start;
	struct timespec end;
	size_t diff_ns;
	size_t diff_us;
	size_t	i;

	i = 0;
	while (1)
	{
		timespec_get(&start, TIME_UTC);
		ft_usleep(500);
		timespec_get(&end, TIME_UTC);

		diff_ns = (end.tv_sec - start.tv_sec) * 1000000000L + 
				(end.tv_nsec - start.tv_nsec);

		diff_us = diff_ns / 1e6;
		if (diff_us > 550 || diff_us < 400)
			break ;
		printf("%zu\n", diff_us);
		i++;
	}
	printf("i: %zu [%zu]\n", i, diff_us);
	return 0;
}