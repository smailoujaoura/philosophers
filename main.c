/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:37:01 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/08 08:16:43 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eating(t_philo *philo)
{
	if (philo->philo_number % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(&philo->sync->write);
		printf("%zu ms %zu has taken a fork\n", get_time() - philo->sync->starting_time, philo->philo_number);
		pthread_mutex_unlock(&philo->sync->write);
		pthread_mutex_lock(&philo->right_fork);

		pthread_mutex_lock(&philo->sync->lock);
		philo->last_meal = get_time();
		pthread_mutex_unlock(&philo->sync->lock);
		pthread_mutex_lock(&philo->sync->write);
		printf("%zu ms %zu has taken a fork\n", get_time() - philo->sync->starting_time, philo->philo_number);
		printf("%zu ms %zu is eating\n", get_time() - philo->sync->starting_time, philo->philo_number);
		pthread_mutex_unlock(&philo->sync->write);
	}
	else
	{
		pthread_mutex_lock(&philo->right_fork);

		pthread_mutex_lock(&philo->sync->write);
		printf("%zu ms %zu has taken a fork\n", get_time() - philo->sync->starting_time, philo->philo_number);
		pthread_mutex_unlock(&philo->sync->write);

		pthread_mutex_lock(philo->left_fork);

		pthread_mutex_lock(&philo->sync->lock);
		philo->last_meal = get_time();
		pthread_mutex_unlock(&philo->sync->lock);

		pthread_mutex_lock(&philo->sync->write);
		printf("%zu ms %zu has taken a fork\n", get_time() - philo->sync->starting_time, philo->philo_number);
		printf("%zu ms %zu is eating\n", get_time() - philo->sync->starting_time, philo->philo_number);
		pthread_mutex_unlock(&philo->sync->write);
	}

	ft_msleep(philo->sync->eat_time);

	pthread_mutex_unlock(&philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);

	pthread_mutex_lock(&philo->sync->lock);
	philo->total_meals++;
	pthread_mutex_unlock(&philo->sync->lock);
}

void	*start_routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo*)ptr;
	while (1)
	{
		eating(philo);

		pthread_mutex_lock(&philo->sync->write);
		printf("%zu ms %zu is sleeping\n", get_time() - philo->sync->starting_time, philo->philo_number);
		pthread_mutex_unlock(&philo->sync->write);
		ft_msleep(philo->sync->sleep_time);

		pthread_mutex_lock(&philo->sync->write);
		printf("%zu ms %zu is thinking\n", get_time() - philo->sync->starting_time, philo->philo_number);
		pthread_mutex_unlock(&philo->sync->write);
	}
	return (NULL);
}

int	count_eaten_meals(t_philo *philos, t_sync *sync)
{
	int		i;

	i = 0;
	while (i < sync->total_philos)
	{
		if (philos[i].total_meals < sync->cycles_total)
		{
			return (0);
		}
		i++;
	}
	return (1);
}

int	has_reached_total_cycles(t_philo *philos, t_sync *sync)
{
	int	i;

	i = 0;
	while (i < sync->total_philos)
	{
		pthread_mutex_lock(&sync->lock);
		if (philos[i].total_meals < sync->cycles_total)
		{
			pthread_mutex_unlock(&sync->lock);
			return (0);
		}
		pthread_mutex_unlock(&sync->lock);
		i++;
	}
	return (1);
}

int	start_sync(t_philo *philos, t_sync *sync)
{
	int	i;

	i = -1;
	// timer_stamper(1);
	sync->starting_time = get_time();
	while (++i < sync->total_philos)
	{
		philos[i].last_meal = get_time();
		if (pthread_create(&philos[i].thread_id, NULL, start_routine, &philos[i]))
			return (-1);
		if (pthread_detach(philos[i].thread_id) != 0)
			return (-1);
	}
	while (1)
	{
		i = -1;
		while (++i < sync->total_philos)
		{
			pthread_mutex_lock(&sync->lock);
			if (get_time() - philos[i].last_meal > sync->death_time)
			{
				//sync->stop = 1;
				pthread_mutex_lock(&sync->write);
				printf("%zu ms %zu has died\n", get_time() - sync->starting_time, philos[i].philo_number);
				return (0);
			}
			pthread_mutex_unlock(&sync->lock);
		}
		if (sync->cycles_total > 0 && has_reached_total_cycles(philos, sync))
		{
			//sync->stop = 1;
			return (0);
		}
		usleep(1000);
	}
	pthread_mutex_unlock(&sync->write);
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
		}
	}
	else
		printf("Erroneous Arguments!\n");
	ft_malloc(0, 1, NULL, NULL);
	return (0);
}
