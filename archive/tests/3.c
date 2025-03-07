#include <stdio.h>
#include <unistd.h>
#include <bits/types.h>
#include <sys/time.h>
#include <time.h>

# define TIMES 1000

int main(void)
{
	struct timespec	start;
	struct timespec	end;
	struct timeval	tv;

	timespec_get(&start, TIME_UTC);
	for (int i = 0; i < TIMES; i++)
		gettimeofday(&tv, NULL);
	timespec_get(&end, TIME_UTC);

	printf("[%lf]\n", (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3);
}