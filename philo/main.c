/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:37:01 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/09 13:58:36 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	are_full(t_philo *philos, t_sync *sync)
{
	int	i;

	i = 0;
	pthread_mutex_lock(sync->checking_mutex);
	while (i < sync->total_philos)
	{
		if (philos[i].total < sync->total_cycles)
		{
			pthread_mutex_unlock(sync->checking_mutex);
			return (0);
		}
		i++;
	}
	pthread_mutex_unlock(sync->checking_mutex);
	return (1);
}

void	write_safely(t_philo *philo, char *message)
{
	pthread_mutex_lock(philo->sync->writing_mutex);
	printf("%zu ms %d %s\n", get_time() - philo->sync->start_time, philo->number, message);
	pthread_mutex_unlock(philo->sync->writing_mutex);
}

void	*start_routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo*)ptr;
	while (1)
	{
		pthread_mutex_lock(philo->first);
		write_safely(philo, "has taken a fork");
		pthread_mutex_lock(philo->second);
		write_safely(philo, "has taken a fork");

		pthread_mutex_lock(philo->sync->checking_mutex);
		philo->last = get_time();
		pthread_mutex_unlock(philo->sync->checking_mutex);

		write_safely(philo, "is eating");
		ft_msleep(philo->sync->eat_time);

		pthread_mutex_unlock(philo->second);
		pthread_mutex_unlock(philo->first);

		pthread_mutex_lock(philo->sync->checking_mutex);
		philo->total++;
		pthread_mutex_unlock(philo->sync->checking_mutex);

		write_safely(philo, "is sleeping");
		ft_msleep(philo->sync->sleep_time);
		write_safely(philo, "is thinking");
	}
	return (NULL);
}

void	start_sync(t_philo *philos, t_sync *sync, int i)
{
	sync->start_time = get_time();
	while (++i < sync->total_philos)
	{
		philos[i].last = get_time();
		if (pthread_create(&philos[i].thread, NULL, start_routine, &philos[i]))
			return ;
		if (pthread_detach(philos[i].thread))
			return ;
	}
	while (1)
	{
		i = -1;
		while (++i < sync->total_philos)
		{
			pthread_mutex_lock(sync->checking_mutex);
			if (get_time() - philos[i].last >= sync->death_time)
			{
				write_safely(&philos[i], "has died");
				pthread_mutex_lock(sync->writing_mutex);
				return ;
			}
			pthread_mutex_unlock(sync->checking_mutex);
		}
		if (sync->total_cycles > 0 && are_full(philos, sync))
			break ;
		usleep(1000);
	}
}

int	main(int argc, char *argv[])
{
	static t_sync	sync;
	static t_philo	philos[MAX_PHILOS];

	if (argc == 5 || argc == 6)
	{
		if (check_args(&sync, argv, argc == 6) == -1)
			return (1);
		else if (sync.total_philos > MAX_PHILOS)
			printf("Max philosophers allowed is %d\n", MAX_PHILOS);
		else
		{
			if (init_sync(&sync, philos) == -1)
				return (1);
			start_sync(philos, &sync, -1);
		}
	}
	return (0);
}
