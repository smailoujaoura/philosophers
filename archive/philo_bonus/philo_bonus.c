/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:54:20 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/19 11:53:06 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	write_safely(t_philo *philo, char *message)
{
	sem_wait(philo->sync->write_sem);
	printf("%zu %d %s\n",
		get_time() - philo->sync->start_time, philo->number, message);
	sem_post(philo->sync->write_sem);
}

void	ft_eat(t_philo *philo, int *posted)
{
	if (philo->number % 2 == 0)
		sem_wait(philo->sync->resources_sem);
	sem_wait(philo->sync->forks_sem);
	write_safely(philo, "has taken a fork");
	sem_wait(philo->sync->forks_sem);
	write_safely(philo, "has taken a fork");
	sem_wait(philo->sync->death_sem);
	philo->last = get_time();
	sem_post(philo->sync->death_sem);
	if (philo->number % 2)
		sem_post(philo->sync->resources_sem);
	write_safely(philo, "is eating");
	ft_msleep(philo->sync->eat_time);
	sem_post(philo->sync->forks_sem);
	sem_post(philo->sync->forks_sem);
	philo->total++;
	if (philo->sync->total_cycles > 0
		&& philo->total >= philo->sync->total_cycles && !*posted)
	{
		sem_post(philo->sync->meals_sem);
		*posted = 1;
	}
}

void	*exit_on_death(void *ptr)
{
	t_philo	*philo;

	philo = ptr;
	while (true)
	{
		sem_wait(philo->sync->death_sem);
		if (get_time() - philo->last > philo->sync->death_time)
		{
			sem_wait(philo->sync->write_sem);
			printf("%zu %i died\n",
				get_time() - philo->sync->start_time, philo->number);
			exit(0);
		}
		sem_post(philo->sync->death_sem);
	}
	return (NULL);
}

void	philosopher(t_philo *philo)
{
	pthread_t	tid;
	int			posted;

	posted = 0;
	philo->last = get_time();
	if (pthread_create(&tid, NULL, exit_on_death, philo))
		exit(1);
	pthread_detach(tid);
	while (true)
	{
		ft_eat(philo, &posted);
		write_safely(philo, "is sleeping");
		ft_msleep(philo->sync->sleep_time);
		write_safely(philo, "is thinking");
	}
}
