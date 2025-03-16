/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:37:01 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/16 18:10:25 by soujaour         ###   ########.fr       */
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

bool	write_safely(t_philo *philo, char *message)
{
	if (end_simulation(0, philo->sync))
		return (true);
	pthread_mutex_lock(&philo->sync->write_mutex);
	printf("%zu ms %d %s\n", get_time() - philo->sync->start_time, philo->number, message);
	pthread_mutex_unlock(&philo->sync->write_mutex);
	return (false);
}

bool	ft_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->first);
	if (write_safely(philo, "has taken a fork"))
	{
		pthread_mutex_unlock(philo->first);
		return (false);
	}
	pthread_mutex_lock(philo->second);
	if (write_safely(philo, "has taken a fork"))
	{
		pthread_mutex_unlock(philo->first);
		pthread_mutex_unlock(philo->second);
		return (false);
	}
	pthread_mutex_lock(&philo->sync->death_mutex);
	philo->last = get_time();
	pthread_mutex_unlock(&philo->sync->death_mutex);
	if (write_safely(philo, "is eating"))
		return (false);
	ft_msleep(philo->sync->eat_time);
	pthread_mutex_unlock(philo->second);
	pthread_mutex_unlock(philo->first);
	pthread_mutex_lock(&philo->sync->meals_mutex);
	philo->total++;
	pthread_mutex_unlock(&philo->sync->meals_mutex);
	return (true);
}

bool	ft_think(t_philo *philo)
{
	if (write_safely(philo, "is thinking"))
		return (false);
	return (true);
}

bool	ft_sleep(t_philo *philo)
{
	if (write_safely(philo, "is sleeping"))
		return (false);
	ft_msleep(philo->sync->sleep_time);
	return (true);
}

void	*philosopher(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	while (true)
	{
		if (!ft_eat(philo))
			break ;
		if (!ft_sleep(philo))
			break ;
		if (!ft_think(philo))
			break ;
	}
	return (NULL);
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
	if (pthread_create(&sync->monitor_tpid, NULL, monitor, sync))
		return (-1); // STILL 
	pthread_join(sync->monitor_tpid, NULL);
	// monitor(sync);
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
