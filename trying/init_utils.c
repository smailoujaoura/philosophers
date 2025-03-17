/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:56:55 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/17 12:01:39 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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


void	destroy_and_free(t_sync *sync)
{
	int	i;

	i = -1;
	while (++i < sync->total_philos)
	{
		pthread_mutex_destroy(&((sync->philos_ptr)[i].right));
	}
	pthread_mutex_destroy(&sync->write_mutex);
	pthread_mutex_destroy(&sync->meals_mutex);
	pthread_mutex_destroy(&sync->death_mutex);
	pthread_mutex_destroy(&sync->end_mutex);
	free(sync->philos_ptr);
}

void	finalize_init(t_philo *philos, int total_philos)
{
	int	i;

	i = -1;
	while (++i < total_philos)
	{
		if (i == 0)
			philos[i].left = &philos[total_philos - 1].right;
		else
			philos[i].left = &philos[i - 1].right;
	}
	i = -1;
	while (++i < total_philos)
	{
		if (philos[i].number % 2 == 0)
		{
			philos[i].first = philos[i].left;
			philos[i].second = &philos[i].right;
		}
		else
		{
			philos[i].first = &philos[i].right;
			philos[i].second = philos[i].left;
		}
	}
}

int	init_sync(t_sync *sync, t_philo **ptr, int i)
{
	t_philo	*philos;

	philos = malloc(sizeof(t_philo) * sync->total_philos);
	if (philos == NULL)
		return (-1);
	*ptr = philos;
	while (++i < sync->total_philos)
	{
		if (pthread_mutex_init(&philos[i].right, NULL))
		{
			while (--i > -1)
			{
				pthread_mutex_destroy(&philos[i].right);
			}
			init_mutexes(sync, DESTROY, NULL);
			free(philos);
			return (-1);
		}
		philos[i].number = i + 1;
		philos[i].sync = sync;
	}
	sync->philos_ptr = philos;
	finalize_init(philos, sync->total_philos);
	return (0);
}

int	init_mutexes(t_sync *sync, int action, int *arr)
{
	if (action == INITIALIZE)
	{
		arr[0] = pthread_mutex_init(&sync->write_mutex, NULL);
		arr[1] = pthread_mutex_init(&sync->meals_mutex, NULL);
		arr[2] = pthread_mutex_init(&sync->death_mutex, NULL);
		arr[3] = pthread_mutex_init(&sync->end_mutex, NULL);
		if (arr[0] == -1)
			pthread_mutex_destroy(&sync->write_mutex);
		if (arr[1] == -1)
			pthread_mutex_destroy(&sync->meals_mutex);
		if (arr[2] == -1)
			pthread_mutex_destroy(&sync->death_mutex);
		if (arr[3] == -1)
			pthread_mutex_destroy(&sync->end_mutex);
	}
	if (action == DESTROY)
	{
		pthread_mutex_destroy(&sync->write_mutex);
		pthread_mutex_destroy(&sync->meals_mutex);
		pthread_mutex_destroy(&sync->death_mutex);
		pthread_mutex_destroy(&sync->end_mutex);
	}
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
	if (error || !sync->death_time || !sync->eat_time
		|| !sync->sleep_time || !sync->total_cycles)
		return (-1);
	return (0);
}
