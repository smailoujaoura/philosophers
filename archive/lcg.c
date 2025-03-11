#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

size_t	getrandomint(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000 + tv.tv_usec);
}

char	*prng(void)
{
	char	*name = malloc(sizeof(char) * 31);
	int		i = 0;
	size_t	seed = getrandomint();
	size_t	key;

	while (i < 30)
	{
		key = (seed * 1664525 + 1013904223) % 62;
		seed = (seed * 1664525 + 1013904223) * 8276348762;
		if (key < 26)
			name[i] = 'A' + key % 26;
		else if (key >= 26 && key < 52)
			name[i] = 'a' + key % 26;
		else
			name[i] = '0' + key % 10;
		i++;
	}
	name[i] = '\0';
	return (name);
}

int main(void)
{
	char	*random_name;

	random_name = prng();
	printf("[%s]\n", random_name);
}