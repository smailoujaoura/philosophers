/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:37:01 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/18 20:27:46 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	write_safely(t_philo *philo, char *message)
{
	sem_wait(philo->sync->write_sem);
	printf("%zu ms %d %s\n",
		get_time() - philo->sync->start_time, philo->number, message);
	sem_post(philo->sync->write_sem);
}

void	*exit_on_death(void *ptr)
{
	t_philo	*philo;

	philo = ptr;
	while (true)
	{
		usleep(100);
		if (get_time() - philo->last > philo->sync->death_time)
		{
			sem_wait(philo->sync->write_sem);
			printf("%zu %i died\n", get_time() - philo->sync->start_time, philo->number);
			exit(0);
		}
	}
	return (NULL);
}

void	philosopher(t_philo *philo)
{
	pthread_t	tid;
	int			posted;

	posted = 0;
	if (philo->number % 2 == 0)
		ft_msleep(philo->sync->eat_time / 2);
	philo->last = get_time();

	if (pthread_create(&tid, NULL, exit_on_death, philo))
		exit(1);
	pthread_detach(tid);
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
		if (!posted && philo->sync->total_cycles > 0 && philo->total >= philo->sync->total_cycles)
		{
			sem_post(philo->sync->meals_sem);
			posted = 1;
		}
		write_safely(philo, "is sleeping");
		ft_msleep(philo->sync->sleep_time);
		write_safely(philo, "is thinking");
	}
}

void	*count_meals(void *ptr)
{
	t_sync	*sync;
	int		i;

	i = -1;
	sync = ptr;
	while (++i < sync->total_philos)
	{
		sem_wait(sync->meals_sem);
	}
	kill(sync->pids[0], SIGTERM);
	return (NULL);
}

int	start_sync(t_philo *philos, t_sync *sync, int i)
{
	int	status;

	if (sync->total_cycles > 0)
	{
		if (pthread_create(&sync->monitor, NULL, count_meals, sync))
			return (-1);
		pthread_detach(sync->monitor);
	}
	sync->start_time = get_time();
	while (++i < sync->total_philos)
	{
		sync->pids[i] = fork();
		if (sync->pids[i] == -1)
		{
			while (--i)
			{
				kill(sync->pids[i], SIGTERM);
				waitpid(sync->pids[i], NULL, 0);
			}
			return (-1);
		}
		else if (sync->pids[i] == 0)
			philosopher(&philos[i]);
	}
	waitpid(-1, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGTERM)
	{
		i = 0;
		while (++i < sync->total_philos)
			kill(sync->pids[i], SIGTERM);
		printf("here STOPPED DUE TO MEAL LIMIT\n");
	}
	else
	{
		i = -1;
		while (++i < sync->total_philos)
			kill(sync->pids[i], SIGTERM);
		printf("OTHER\n");
	}
	while (waitpid(-1, NULL, 0) > 0)
		;
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
