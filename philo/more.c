/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:56:55 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/09 13:42:02 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	check_args(t_sync *sync, char *argv[], int optional)
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
	if (error || !sync->death_time || !sync->eat_time || !sync->sleep_time || !sync->total_cycles)
		return (-1);
	return (0);
}

void	finalize_init(t_philo *philos, int total_philos)
{
	int	i;

	i = -1;
	while (++i < total_philos)
	{
		if (i == 0)
			philos[i].left = philos[total_philos - 1].right;
		else
			philos[i].left = philos[i - 1].right;
	}
	i = -1;
	while (++i < total_philos)
	{
		if (philos[i].number % 2 == 0)
		{
			philos[i].first = philos[i].left;
			philos[i].second = philos[i].right;
		}
		else
		{
			philos[i].first = philos[i].right;
			philos[i].second = philos[i].left;
		}
	}
}

int	init_sync(t_sync *sync, t_philo *philos)
{
	int						i;
	static pthread_mutex_t	forks[MAX_PHILOS] = {PTHREAD_MUTEX_INITIALIZER};
	static pthread_mutex_t	writing = PTHREAD_MUTEX_INITIALIZER;
	static pthread_mutex_t	checking = PTHREAD_MUTEX_INITIALIZER;

	sync->writing_mutex = &writing;
	sync->checking_mutex = &checking;
	i = -1;
	while (++i < sync->total_philos)
	{
		philos[i].sync = sync;
		philos[i].number = i + 1;
		philos[i].right = &forks[i];
	}
	finalize_init(philos, sync->total_philos);
	return (0);
}
