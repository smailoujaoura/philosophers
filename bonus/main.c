/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:37:01 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/11 17:16:45 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	are_full(t_philo *philos, t_sync *sync)
{
	int	i;

	i = 0;
	while (i < sync->total_philos)
	{
		sem_wait(sync->checks);
		if (philos[i].total < sync->total_cycles)
		{
			sem_post(sync->checks);
			return (0);
		}
		sem_post(sync->checks);
		i++;
	}
	return (1);
}

void	write_safely(t_philo *philo, char *message)
{
	sem_wait(philo->sync->logs);
	printf("%zu ms %d %s\n", get_time() - philo->sync->start_time, philo->number, message);
	sem_post(philo->sync->logs);
}

void	spawn_philo(t_philo *philo)
{
	while (1)
	{
		sem_wait(philo->sync->checks);
		if (philo->sync->stop)
		{
			sem_post(philo->sync->checks);
			break ;
		}
		sem_post(philo->sync->checks);
		
		sem_wait(philo->sync->forks);
		write_safely(philo, "has taken a fork");
		sem_wait(philo->sync->forks);
		write_safely(philo, "has taken a fork");

		sem_wait(philo->sync->checks);
		philo->last = get_time();
		sem_post(philo->sync->checks);

		write_safely(philo, "is eating");
		ft_msleep(philo->sync->eat_time);

		sem_post(philo->sync->forks);
		sem_post(philo->sync->forks);

		if (philo->sync->total_cycles != -1)
		{
			sem_wait(philo->sync->checks);
			philo->total++;
			sem_post(philo->sync->checks);
		}

		write_safely(philo, "is sleeping");
		ft_msleep(philo->sync->sleep_time);
		write_safely(philo, "is thinking");
	}
	sem_close(philo->sync->logs);
	sem_close(philo->sync->forks);
	sem_close(philo->sync->checks);
	exit(0);
}

int	start_sync(t_philo *philos, t_sync *sync, int i)
{
	sync->start_time = get_time();
	while (++i < sync->total_philos)
	{
		philos[i].last = sync->start_time;
		philos[i].pid = fork();
		if (philos[i].pid == 0)
		{
			spawn_philo(&philos[i]);
		}
		else if (philos[i].pid == -1)
			return (-1);
		else
			continue ;
	}
	static int flag;
	while (1)
	{
		i = -1;
		while (++i < sync->total_philos)
		{
			sem_wait(sync->checks);
			if (get_time() - philos[i].last > sync->death_time)
			{
				sync->stop = 1;
				write_safely(&philos[i], "has died");
				sem_post(sync->checks);
				flag = 1;
				break ;
			}
			sem_post(sync->checks);
		}
		if (flag)
			break ;
		if (sync->total_cycles > 0 && are_full(philos, sync))
		{
			sem_wait(sync->checks);
			sync->stop = 1;
			sem_post(sync->checks);
			break ;
		}
		usleep(100);
	}
	i = -1;
	while (++i < sync->total_philos)
		waitpid(-1, NULL, 0);
	sem_close(sync->logs);
	sem_close(sync->forks);
	sem_close(sync->checks);
	return (0);
}

int	main(int argc, char *argv[])
{
	static t_sync	*sync;
	static t_philo	*philos;

	if (argc == 5 || argc == 6)
	{
		sync = malloc(sizeof(t_sync));
		if (sync == NULL)
			return (1);
		if (check_args(sync, argv, argc == 6) == -1)
			return (1);
		else
		{
			sync->stop = 0;
			philos = malloc(sizeof(t_philo) * sync->total_philos);
			if (philos == NULL)
				return (1);
			if (init_everything(sync, philos, sync->total_philos, -1) == -1)
			{
				perror("failed to sem");
				free(philos);
				return (1);
			}
			if (start_sync(philos, sync, -1) == -1)
				close_and_free(sync, philos);
		}
	}
	else
		printf("Error\n");
	return (0);
}
