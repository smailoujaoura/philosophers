/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:37:01 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/19 12:44:36 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_death(void *ptr)
{
	t_philo	*philos;
	t_arg	*args;
	size_t	i = 0;
	
	philos = (t_philo *)ptr;
	args = philos->args;
	while (1)
	{
		i = 0;
		while (i < args->total_philos)
		{
			if (get_time() - philos[i].last_meal >= args->death_time)
			{
				printf("%zu ms %zu died\n", get_time() - philos->sync->start_time, philos[i].philo_number);
				return ((void *)1);
			}
			i++;
		}
		usleep(100);
	}
	return (NULL);
}

void	*start_routine(void *ptr)
{
	t_philo	*philo;
	philo = (t_philo *)ptr;

	// philo->args->cycles_total != -1 && philo->total_meals != philo->args->cycles_total
	while (1)
	{
		if (philo->philo_number % 2 == 0)
		{
			printf("%zu ms %zu is sleeping\n", timer_stamper(0), philo->philo_number);
			precise_sleep(philo->args->sleep_time);
		}
	}
}

void	*start_sync(t_philo *philos, t_arg *args)
{
	struct timeval	start;
	pthread_t		monitor;
	size_t			i;

	timer_stamper(0)
	while (i < args->total_philos)
	{
		if (pthread_create(&philos[i].thread_id, NULL, start_routine, &philos[i]) != 0)
			return (NULL);
		pthread_detach(philos[i].philo_number);
		i++;
	}
	pthread_create(&monitor, NULL, monitor_death, philos);
	pthread_join(monitor, NULL);
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
