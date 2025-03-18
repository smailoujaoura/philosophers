/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rand_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 10:09:01 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/18 17:09:40 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

size_t	custom_atoi(char *s, int *err)
{
	size_t	num;

	num = 0;
	while ((*s >= 9 && *s <= 13) || *s == 32)
		s++;
	if (*s == '-' || *s == '+')
	{
		if (*s == '-')
			*err = 1;
		s++;
	}
	if (!*s)
		*err = 1;
	while (*s)
	{
		if (num > SIZE_MAX / 10
			|| (num == SIZE_MAX / 10 && (size_t)(*s - '0') > SIZE_MAX % 10))
			*err = 1;
		if (*s >= '0' && *s <= '9')
			num = num * 10 + (*s - '0');
		else
			*err = 1;
		s++;
	}
	return (num);
}


// void	close_and_free(t_sync *sync, t_philo *philos)
// {
// 	free(philos);
// 	sem_close(sync->forks_sem);
// 	sem_close(sync->write_sem);
// 	sem_close(sync->meals_sem);
// 	sem_close(sync->death_sem);
// }

// int	ft_sem_destroy(sem_t *sem, char *name)
// {
// 	sem_close(sem);
// 	sem_unlink(name);
// 	return (-1);
// }

char	*prng(char *name)
{
	static size_t	seed;
	size_t	key;
	int		i;

	i = 0;
	name[i] = '/';
	if (seed == 0)
		seed = get_time();
	else
		seed += get_time();
	while (++i < 20)
	{
		key = (seed * 1664525 + 1013904223) % 62;
		seed = (seed * 1664525 + 1013904223) * 8276348762;
		if (key < 26)
			name[i] = 'A' + key % 26;
		else if (key >= 26 && key < 52)
			name[i] = 'a' + key % 26;
		else
			name[i] = '0' + key % 10;
		seed += 1;
	}
	name[i] = '\0';
	// printf("[%s]\n", name);
	return (name);
}