/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:37:01 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/18 17:15:34 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*exit_on_death(void *ptr)
{
	t_philo	*philo;
	size_t	time;

	philo = ptr;
	while (true)
	{
		time = get_time();
		if (time - philo->last > philo->sync->death_time)
		{
			sem_wait(philo->sync->write_sem);
			printf("%zu ms %i died\n", time - philo->sync->start_time, philo->number);
			exit(0);
		}
		usleep(100);
	}
}

void	write_safely(t_philo *philo, char *message)
{
	sem_wait(philo->sync->write_sem);
	printf("%zu ms %d %s\n",
		get_time() - philo->sync->start_time, philo->number, message);
	sem_post(philo->sync->write_sem);
}

void	philosopher(t_philo *philo)
{
	pthread_t	tid;

	printf("PID: %d\n", getpid());
	if (philo->number % 2 == 0)
		usleep(500);
	philo->last = get_time();
	if (pthread_create(&tid, NULL, exit_on_death, philo))
		exit(1);
	while (true)
	{
		sem_wait(philo->sync->forks_sem);
		write_safely(philo, "has taken a fork");
		sem_wait(philo->sync->forks_sem);
		write_safely(philo, "has taken a fork");
		philo->last = get_time();
		write_safely(philo, "is eating");

		ft_msleep(philo->sync->eat_time);

		sem_post(philo->sync->forks_sem);
		sem_post(philo->sync->forks_sem);
		philo->total++;

		if (philo->sync->total_cycles > 0 && philo->total >= philo->sync->total_cycles)
		{
			sem_post(philo->sync->meals_sem);
			exit(0);
		}
		write_safely(philo, "is sleeping");
		ft_msleep(philo->sync->sleep_time);

		write_safely(philo, "is thinking");
	}
}

void	*monit(void *ptr)
{
	(void)ptr;
   	waitpid(-1, NULL, 0);
	kill(0, SIGKILL);
    while (waitpid(-1, NULL, 0) > 0)
		;
    return (NULL);
}


void	watch_meals(t_sync *sync)
{
	int	i;

	if (sync->total_cycles < 0)
		return ;
	i = -1;
	while (++i < sync->total_philos)
	{
		sem_wait(sync->meals_sem);
	}
}

int	start_sync(t_philo *philos, t_sync *sync, int i)
{
	if (pthread_create(&sync->monitor, NULL, monit, sync))
		return (-1);
	pthread_detach(sync->monitor);
	sync->start_time = get_time();
	while (++i < sync->total_philos)
	{
		sync->pids[i] = fork();
		if (sync->pids[i] == -1)
		{
			while (--i)
			{
				kill(sync->pids[i], SIGKILL);
				waitpid(sync->pids[i], NULL, 0);
			}
			return (-1);
		}
		else if (sync->pids[i] == 0)
			philosopher(&philos[i]);
	}
	watch_meals(sync);
	return (0);
}

void	close_and_free(t_sync *sync)
{
	sem_close(sync->write_sem);
	sem_close(sync->meals_sem);
	sem_close(sync->death_sem);
	sem_close(sync->forks_sem);
	free(sync->pids);
	free(sync->philos_ptr);
}

int	main(int argc, char *argv[])
{
	t_sync	sync;
	t_philo	*philos;

	philos = NULL;
	printf("Starting main PID: %d\n", getpid());
	if (argc == 5 || argc == 6)
	{
		if (check_args(&sync, argv, argc == 6) == -1)
			return (printf("Wrong arguments\n"), 1);
		if (argc == 6 && sync.total_cycles == 0)
			return (0);
		if (init_sems(&sync, sync.total_philos) == -1)
			return (printf("Something wrong occured\n"), free(sync.pids), 1);
		if (init_sync(&sync, &philos) == -1)
			return (printf("Something wrong occured\n"), free(sync.pids), 1);
		if (start_sync(philos, &sync, -1) == -1)
			return (close_and_free(&sync), 1);
		return (close_and_free(&sync), 0);
	}
	printf("Wrong arguments number\n");
	return (1);
}

// check status of each process if it from different contexts and do what is needed depenedent on that.