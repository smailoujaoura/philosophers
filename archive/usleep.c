#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

int main(void)
{
	struct timeval	tv1;
	struct timeval	tv2;
	
	while (1)
	{
		gettimeofday(&tv1, NULL);
		usleep(100);
		gettimeofday(&tv2, NULL);
		printf("%ld %ld\n", tv1.tv_sec, tv1.tv_usec);
		printf("%ld %ld\n", tv2.tv_sec, tv2.tv_usec);
		printf("what [%ld] diff: [%ld]\n", tv2.tv_sec - tv1.tv_sec, tv2.tv_usec - tv1.tv_usec);
		printf("slept: \t[[[[[[[[%ld]]]\n\n\n\n", (tv2.tv_sec - tv1.tv_sec) * 1000000 + (tv2.tv_usec - tv1.tv_usec));
		if (tv2.tv_usec - tv1.tv_usec < 100) // && tv2.tv_sec - tv1.tv_sec != 1
		{
			printf("broke out\n");
			break;
		}
	}
}