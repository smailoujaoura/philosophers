/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:56:55 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/19 11:52:31 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	bulk_unlink(char names[5][21])
{
	sem_unlink(names[0]);
	sem_unlink(names[1]);
	sem_unlink(names[2]);
	sem_unlink(names[3]);
	sem_unlink(names[4]);
}

int	close_and_unlink(t_sync *sync, int index)
{
	if (index == 1)
		sem_close(sync->forks_sem);
	else if (index == 2)
	{
		sem_close(sync->forks_sem);
		sem_close(sync->write_sem);
	}
	else if (index == 3)
	{
		sem_close(sync->forks_sem);
		sem_close(sync->write_sem);
		sem_close(sync->death_sem);
	}
	else if (index == 4)
	{
		sem_close(sync->forks_sem);
		sem_close(sync->write_sem);
		sem_close(sync->death_sem);
		sem_close(sync->meals_sem);
	}
	while (--index)
		sem_unlink(sync->name[index]);
	return (-1);
}

int	init_sems(t_sync *sync, int num_philo)
{
	random_name(sync->name[0]);
	random_name(sync->name[1]);
	random_name(sync->name[2]);
	random_name(sync->name[3]);
	random_name(sync->name[4]);
	sync->forks_sem = sem_open(sync->name[0], O_CREAT, 0666, num_philo);
	if (sync->forks_sem == SEM_FAILED)
		return (-1);
	sync->write_sem = sem_open(sync->name[1], O_CREAT, 0666, BINARY);
	if (sync->write_sem == SEM_FAILED)
		return (close_and_unlink(sync, 1));
	sync->death_sem = sem_open(sync->name[2], O_CREAT, 0666, BINARY);
	if (sync->death_sem == SEM_FAILED)
		return (close_and_unlink(sync, 2));
	sync->meals_sem = sem_open(sync->name[3], O_CREAT, 0666, SERIALIZER);
	if (sync->meals_sem == SEM_FAILED)
		return (close_and_unlink(sync, 3));
	sync->resources_sem = sem_open(sync->name[4], O_CREAT, 0666, SERIALIZER);
	if (sync->resources_sem == SEM_FAILED)
		return (close_and_unlink(sync, 4));
	bulk_unlink(sync->name);
	return (0);
}

int	check_args(t_sync *sync, char **argv, int optional)
{
	int		error;

	error = 0;
	sync->total_philos = custom_atoi(argv[1], &error);
	sync->death_time = custom_atoi(argv[2], &error);
	sync->eat_time = custom_atoi(argv[3], &error);
	sync->sleep_time = custom_atoi(argv[4], &error);
	sync->total_cycles = -1;
	if (optional)
		sync->total_cycles = custom_atoi(argv[5], &error);
	if (error)
		return (-1);
	if (sync->death_time <= 0)
		return (-1);
	if (sync->eat_time <= 0)
		return (-1);
	if (sync->sleep_time <= 0)
		return (-1);
	if (optional && sync->total_cycles < 0)
		return (-1);
	sync->pids = malloc(sizeof(pid_t) * sync->total_philos);
	if (sync->pids == NULL)
		return (-1);
	return (0);
}

int	init_sync(t_sync *sync, t_philo **ptr)
{
	t_philo	*philos;
	int		i;

	philos = malloc(sizeof(t_philo) * sync->total_philos);
	if (philos == NULL)
		return (-1);
	i = -1;
	while (++i < sync->total_philos)
	{
		philos[i].number = i + 1;
		philos[i].sync = sync;
	}
	sync->philos_ptr = philos;
	*ptr = philos;
	return (0);
}
