/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 21:08:21 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/09 10:23:03 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	safe_writing_messages(t_philo *philo, char *message, int f)
{
	static int	flag;

	if (f == -1)
	{
		pthread_mutex_lock(&philo->sync->stats);
		flag = 1;
		pthread_mutex_unlock(&philo->sync->stats);
		pthread_mutex_lock(&philo->sync->write);
		if (message)
			printf("%zu ms %zu %s\n", get_time() - philo->sync->starting_time, philo->philo_number, message);
	}
	else if (f == 2)
	{
		pthread_mutex_lock(&philo->sync->stats);
		return (flag);
		pthread_mutex_unlock(&philo->sync->stats);
	}
	pthread_mutex_lock(&philo->sync->stats);
	if (flag == 0)
	{
		pthread_mutex_unlock(&philo->sync->stats);
		pthread_mutex_lock(&philo->sync->write);
		printf("%zu ms %zu %s\n", get_time() - philo->sync->starting_time, philo->philo_number, message);
		pthread_mutex_unlock(&philo->sync->write);
		return (0);
	}
	return (1);
}