/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:37:01 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/09 10:21:03 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// int	get_mutex(pthread_mutex_t *mutex, int flag)
// {
// 	static int	i;

// 	if ()
// }

int	lock_if(t_philo *philo, pthread_mutex_t *mutex)
{
	if (safe_writing_messages(philo, NULL, 2))
	{
		if (mutex == philo->second)
		{
			pthread_mutex_unlock(philo->first);
			philo->has_first = 0;
		}
		return (1);
	}
	pthread_mutex_lock(mutex);
	return (0);
}

int	eating(t_philo *philo)
{
	if (lock_if(philo, philo->first))
		return (1);
	philo->has_first = 1;

	if (safe_writing_messages(philo, "has taken a fork", 0))
		return (1);

	if (lock_if(philo, philo->second))
		return (1);
	philo->has_second = 1;

	if (safe_writing_messages(philo, "has taken a fork", 0))
		return (2);

	pthread_mutex_lock(&philo->sync->checking);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->sync->checking);
	

	if (safe_writing_messages(philo, "is eating", 0))
		return (3);


	ft_msleep(philo->sync->eat_time);

	pthread_mutex_unlock(philo->second);
	pthread_mutex_unlock(philo->first);
	philo->has_first = 0;
	philo->has_second = 0;


	pthread_mutex_lock(&philo->sync->checking);
	philo->total_meals++;
	pthread_mutex_unlock(&philo->sync->checking);
	return (0);
}

void	release_held_forks(t_philo *philo)
{
	if (philo->has_first == 1)
		pthread_mutex_unlock(philo->first);
	if (philo->has_second == 1)
		pthread_mutex_unlock(philo->second);
}

void	*start_routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo*)ptr;
	while (1)
	{
		if (eating(philo))
			break ;

		if (safe_writing_messages(philo, "is sleeping", 0))
			break ;

		ft_msleep(philo->sync->sleep_time);

		if (safe_writing_messages(philo, "is thinking", 0))
			break ;

		pthread_mutex_lock(&philo->sync->stopper);
		if (philo->sync->stop == 1)
		{
			pthread_mutex_unlock(&philo->sync->stopper);
			break ;
		}
		pthread_mutex_unlock(&philo->sync->stopper);
	}
	release_held_forks(philo);
	return (NULL);
}

int	has_reached_total_cycles(t_philo *philos, t_sync *sync)
{
	int	i;

	i = 0;
	while (i < sync->total_philos)
	{
		pthread_mutex_lock(&sync->checking);
		if (philos[i].total_meals < sync->cycles_total)
		{
			// printf("here %i\n", i);
			pthread_mutex_unlock(&sync->checking);
			return (0);
		}
		pthread_mutex_unlock(&sync->checking);
		i++;
	}
	return (1);
}

void	*monit(void *ptr)
{
	t_philo	*philos;
	t_sync	*sync;
	int		i;

	philos = (t_philo*)ptr;
	sync = (*philos).sync;
	while (1)
	{
		i = -1;
		while (++i < sync->total_philos)
		{
			pthread_mutex_lock(&sync->checking);
			if (get_time() - philos[i].last_meal >= sync->death_time)
			{
				safe_writing_messages(&philos[i], "has died", -1);

				pthread_mutex_unlock(&sync->checking);
				pthread_mutex_lock(&sync->stopper);
				sync->stop = 1;
				pthread_mutex_unlock(&sync->stopper);

				return (0);
			}
			pthread_mutex_unlock(&sync->checking);
		}
		if (sync->cycles_total > 0 && has_reached_total_cycles(philos, sync))
		{
			safe_writing_messages(&philos[0], NULL, -1);
			pthread_mutex_lock(&sync->stopper);
			sync->stop = 1;
			pthread_mutex_unlock(&sync->stopper);
			return (0);
		}
		usleep(1000);
	}
}

int	start_sync(t_philo *philos, t_sync *sync)
{
	int			i;
	pthread_t	monitor;

	i = -1;
	// timer_stamper(1);
	sync->starting_time = get_time();
	while (++i < sync->total_philos)
	{
		philos[i].last_meal = get_time();
		if (pthread_create(&philos[i].thread_id, NULL, start_routine, &philos[i]))
			return (-1);
		// pthread_join(philos[i].thread_id, NULL);
	}
	pthread_create(&monitor, NULL, monit, philos);
	i = -1;
	while (++i < sync->total_philos)
		pthread_join(philos[i].thread_id, NULL);
	pthread_join(monitor, NULL);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_sync	*sync;
	t_philo	*philos;
	
	if (argc == 5 || argc == 6)
	{
		sync = check_args(argv, argc == 6);
		if (sync == NULL || (argc == 6 && sync->cycles_total == 0))
			philos = NULL;
		else if (sync->total_philos > TOTAL_PHILOS)
			printf("Maximum number of philos is 200\n");
		else
		{
			philos = init_sync(sync);
			start_sync(philos, sync);
			// pthread_mutex_unlock(&sync->checking);
			// pthread_mutex_unlock(&sync->write);
			// pthread_mutex_unlock(&sync->state);
			// pthread_mutex_unlock(&sync->stats);
			// pthread_mutex_unlock(&sync->stopper);

			int i = -1;
			while (++i < sync->total_philos)
				pthread_mutex_destroy(philos[i].first);


			// pthread_mutex_destroy(&sync->checking);
			// pthread_mutex_destroy(&sync->write);
			// pthread_mutex_destroy(&sync->state);
			// pthread_mutex_destroy(&sync->stats);
			// pthread_mutex_destroy(&sync->stopper);
		}
	}
	else
	ft_malloc(0, 1, NULL, NULL);
	return (0);
}
