/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:37:01 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/19 14:44:38 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_sleep(t_philo *philo)
{
	printf("%zu %zu is sleeping\n", timer_stamper(0), philo->philo_number);
	precise_sleep(philo->args->sleep_time);
}

void	think(t_philo *philo)
{
	printf("%zu %zu is thinking\n", timer_stamper(0), philo->philo_number);
}

void	eat(t_philo *philo)
{
	if (philo->philo_number % 2 == 0)
	{
		pthread_mutex_lock(&philo->right_fork);
		pthread_mutex_lock(philo->left_fork);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(&philo->right_fork);
	}
	printf("%zu %zu has taken a fork\n", timer_stamper(0), philo->philo_number);
	printf("%zu %zu has taken a fork\n", timer_stamper(0), philo->philo_number);
	pthread_mutex_lock(&philo->sync->status_change);
	philo->last_meal = timer_stamper(0);
	pthread_mutex_unlock(&philo->sync->status_change);

	
	printf("%zu %zu is eating\n", timer_stamper(0), philo->philo_number);
	precise_sleep(philo->args->eat_time);
	pthread_mutex_lock(&philo->sync->status_change);
	philo->total_meals++;
	pthread_mutex_unlock(&philo->sync->status_change);

	
	pthread_mutex_unlock(&philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	*monitor_death(void *ptr)
{
	t_philo	*philos;
	t_arg	*args;
	t_sync	*sync;
	size_t	i;
	int		status;

	philos = (t_philo *)ptr;
	args = philos[0].args;
	sync = philos[0].sync;
	while (1)
	{
		i = 0;
		while (i < args->total_philos)
		{
			pthread_mutex_lock(&sync->status_change);
			status = (timer_stamper(0) - philos[i].last_meal) > args->death_time;
			pthread_mutex_unlock(&sync->status_change);
			if (status)
			{
				printf("Death\n");
				sync->stop = 1;
				return (NULL);
			}
			i++;
		}
		precise_sleep(500);
	}
	return (NULL);
}

void	*start_routine(void *ptr)
{
	t_philo	*philo;
	philo = (t_philo *)ptr;

	while (philo->sync->stop != 1)
	{
		if (philo->philo_number % 2 == 0)
		{
			precise_sleep(philo->args->eat_time);
		}
		eat(philo);
		ft_sleep(philo);
		think(philo);
		if (philo->total_meals == philo->args->cycles_total)
		{
			pthread_mutex_lock(&philo->sync->status_change);
			philo->sync->stop = 1;
			pthread_mutex_unlock(&philo->sync->status_change);
		}
	}
	return (NULL);
}

void	*start_sync(t_philo *philos, t_arg *args)
{
	struct timeval	start __attribute__((unused));
	pthread_t		monitor;
	size_t			i;

	i = 0;
	timer_stamper(1);
	while (i < args->total_philos)
	{
		if (pthread_create(&philos[i].thread_id, NULL, start_routine, &philos[i]) != 0)
		{
			printf("pthread create error\n");
			return (NULL);
		}
		if (pthread_detach(philos[i].thread_id) != 0)
		{
			printf("Pthread detach error\n");
			return (NULL);
		}
		i++;
	}
	pthread_create(&monitor, NULL, monitor_death, philos);
	pthread_join(monitor, NULL);
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_arg	*args;
	t_philo	*philos;
	
	if (argc == 5 || argc == 6)
	{
		args = check_args(argv, argc == 6);
		if (args == NULL || (argc == 6 && args->cycles_total == 0))
			return (1);
		else if (args->total_philos > TOTAL_PHILOS)
			printf("Maximum number of philos is 200\n");
		else
		{
			printf("args: %ld %ld %ld %ld %ld\n", args->total_philos, args->death_time, args->eat_time, args->sleep_time, args->cycles_total);
			philos = init_sync(args);
			start_sync(philos, args);
		}
	}
	else
		printf("Erroneous Arguments!\n");
	ft_malloc(0, 1, NULL, NULL);
	return (0);
}
