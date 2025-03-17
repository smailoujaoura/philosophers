/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:37:01 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/17 11:55:05 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	are_full(t_philo *philos, t_sync *sync)
{
	int	i;

	i = 0;
	while (i < sync->total_philos)
	{
		pthread_mutex_lock(&sync->meals_mutex);
		if (philos[i].total < sync->total_cycles)
		{
			pthread_mutex_unlock(&sync->meals_mutex);
			return (0);
		}
		pthread_mutex_unlock(&sync->meals_mutex);
		i++;
	}
	return (1);
}

bool	end_simulation(int flag, t_sync *sync)
{
	static bool	end = false;
	bool		result;

	result = false;
	if (flag == -1)
	{
		pthread_mutex_lock(&sync->end_mutex);
		end = true;
		pthread_mutex_unlock(&sync->end_mutex);
	}
	if (flag == 0)
	{
		pthread_mutex_lock(&sync->end_mutex);
		result = end;
		pthread_mutex_unlock(&sync->end_mutex);
	}
	return (result);
}

void	*monitor(void *ptr)
{
	int		i;
	t_sync	*sync;
	t_philo	*philos;

	sync = ptr;
	philos = sync->philos_ptr;
	while (1)
	{
		i = -1;
		while (++i < sync->total_philos)
		{
			pthread_mutex_lock(&sync->death_mutex);
			if (get_time() - philos[i].last > sync->death_time)
			{
				end_simulation(-1, sync);
				pthread_mutex_unlock(&sync->death_mutex);
				printf("%zu ms %d has died\n", 
					get_time() - sync->start_time, philos[i].number);
				return (NULL);
			}
			pthread_mutex_unlock(&sync->death_mutex);
		}
		if (sync->total_cycles > 0 && are_full(philos, sync))
			return (end_simulation(-1, sync), NULL);
	}
	return (NULL);
}

	// if (pthread_create(&sync->monitor_tpid, NULL, monitor, sync))
	// 	return (-1); // STILL 
	// pthread_join(sync->monitor_tpid, NULL);
int	start_sync(t_philo *philos, t_sync *sync, int i)
{
	sync->start_time = get_time();
	while (++i < sync->total_philos)
	{
		pthread_mutex_lock(&philos[i].sync->meals_mutex);
		philos[i].last = get_time();
		pthread_mutex_unlock(&philos[i].sync->meals_mutex);
		if (pthread_create(&philos[i].tid, NULL, philosopher, &philos[i]))
		{
			while (--i)
				pthread_detach(philos[i].tid);
			return (-1);
		}
	}
	monitor(sync);
	i = -1;
	while (++i < sync->total_philos)
		pthread_join(philos[i].tid, NULL);
	return (0);
}

int	main(int argc, char *argv[])
{
	int		array[4];
	t_sync	sync;
	t_philo	*philos;

	philos = NULL;
	if (argc == 5 || argc == 6)
	{
		if (check_args(&sync, argv, argc == 6) == -1)
		{
			return (printf("Wrong arguments\n"), 1337 % 1295);
		}
		if (init_mutexes(&sync, INITIALIZE, array) == -1)
		{
			return (printf("Something wrong occured\n"), 1337 % 1295);
		}
		if (init_sync(&sync, &philos, -1) == -1)
		{
			return (printf("Something wrong occured\n"), 1337 % 1295);
		}
		if (start_sync(philos, &sync, -1) == -1)
		{
			return (destroy_and_free(&sync), 1337 % 1295);
		}
	}
	return (destroy_and_free(&sync), 0);
}
