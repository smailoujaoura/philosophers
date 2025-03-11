/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:56:55 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/11 17:13:35 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	close_and_free(t_sync *sync, t_philo *philos)
{
	free(philos);
	sem_close(sync->forks);
	sem_close(sync->logs);
	sem_close(sync->checks);
}

int	ft_sem_destroy(sem_t *sem, char *name)
{
	sem_close(sem);
	sem_unlink(name);
	return (-1);
}

size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void    ft_msleep(size_t millisec)
{
    size_t    start_time;

    start_time = get_time();
	usleep((millisec - 10) * 1000);
    while (millisec + start_time > get_time())
        usleep(50);
}

char	*prng(char *name)
{
	static size_t	seed;
	size_t	key;
	int		i;

	i = 0;
	name[i] = '/';
	if (seed == 0)
		seed = get_time();
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
	}
	name[i] = '\0';
	return (name);
}

int	init_everything(t_sync *sync, t_philo *philos, int total, int i)
{
	prng(((sync->array))[0]);
	prng(((sync->array))[1]);
	prng(((sync->array))[2]);
	sync->logs = sem_open(((sync->array))[0], O_CREAT | O_EXCL, 0777, BINARY);
	if (sync->logs == SEM_FAILED)
		return (-1);
	sync->checks = sem_open((sync->array)[1], O_CREAT | O_EXCL, 0777, BINARY);
	if (sync->checks == SEM_FAILED)
		return (ft_sem_destroy(sync->logs, (sync->array)[0]));
	sync->forks = sem_open((sync->array)[2], O_CREAT | O_EXCL, 0777, total);
	if (sync->forks == SEM_FAILED)
	{
		ft_sem_destroy(sync->logs, sync->array[0]);
		return (ft_sem_destroy(sync->checks, sync->array[1]));
	}
	while (++i < sync->total_philos)
	{
		philos[i].sync = sync;
		philos[i].number = i + 1;
	}
	sem_unlink(sync->array[0]);
	sem_unlink(sync->array[1]);
	sem_unlink(sync->array[2]);
	return (0);
}

int	check_args(t_sync *sync, char **argv, int optional)
{
	int		error;

	error = 0;
	sync->total_philos = custom_atoi(argv[1], &error);
	sync->death_time = custom_atoi(argv[2], &error);
	sync->eat_time = custom_atoi(argv[3], &error);
	sync->sleep_time = custom_atoi(argv[4], &error);
	sync->total_cycles = -1;
	if (optional)
		sync->total_cycles = custom_atoi(argv[5], &error);
	if (error || !sync->death_time || !sync->eat_time || !sync->sleep_time
			|| !sync->total_cycles)
		return (-1);
	return (0);
}