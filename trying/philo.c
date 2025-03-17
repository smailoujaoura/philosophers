/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:54:20 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/17 11:54:50 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

bool	write_safely(t_philo *philo, char *message)
{
	if (end_simulation(0, philo->sync))
		return (true);
	pthread_mutex_lock(&philo->sync->write_mutex);
	printf("%zu ms %d %s\n",
		get_time()- philo->sync->start_time, philo->number, message);
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
	ft_msleep(philo->sync->eat_time, philo->sync);
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
	ft_msleep(philo->sync->sleep_time, philo->sync);
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