#include "philo_bonus.h"

size_t	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (0);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_msleep(size_t millisecs)
{
	size_t	start_time;

	start_time = get_time();
	while (millisecs + start_time > get_time())
	{
		usleep(500);
	}
}

char	*random_name(char *name)
{
	static size_t	seed;
	size_t			key;
	int				i;

	i = 0;
	name[i] = '/';
	if (seed == 0)
		seed = get_time();
	else
		seed += get_time();
	while (++i < 20)
	{
		key = (seed * MULTIPLIER + INCREMENT) % MODULUS;
		if (key < 26)
			name[i] = 'A' + key % 26;
		else if (key >= 26 && key < 52)
			name[i] = 'a' + key % 26;
		else
			name[i] = '0' + key % 10;
		seed *= MULTIPLIER;
	}
	name[i] = '\0';
	return (name);
}

int main(void)
{
	char str[21];
	char *prev = "test";
	int	i;

	i = 0;
	while (true)
	{
		i++;
		printf("iteration: %d ---> [%s]\n", i, random_name(str));
		// if (strcmp(prev, str) == 0)
		// 	break ;
		if (i == 100000)
			break;
		prev = str;
	}
	printf("str: %s\n", str);
	printf("prv: %s\n", prev);
}