/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:56:55 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/08 22:17:11 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void    ft_msleep(size_t millisec)
{
    size_t    start_time;

    start_time = get_time();
	// usleep((millisec - 10) * 1000);
    while (millisec + start_time > get_time())
        usleep(50);
}

t_sync	*check_args(char *argv[], int argc)
{
	int		error;
	t_sync	*sync;

	error = 0;
	sync = ft_malloc(sizeof(t_sync), 0, NULL, NULL);
	memset(sync, 0, sizeof(t_sync));
	sync->total_philos = custom_atoi(argv[1], &error);
	sync->death_time = custom_atoi(argv[2], &error);
	sync->eat_time = custom_atoi(argv[3], &error);
	sync->sleep_time = custom_atoi(argv[4], &error);
	sync->cycles_total = -1;
	if (argc)
		sync->cycles_total = custom_atoi(argv[5], &error);
	if (error || !sync->death_time || !sync->eat_time || !sync->sleep_time)
		return (NULL);
	return (sync);
}

void	*destroy_mutexes(t_philo *philos, size_t i, int flag)
{
	size_t	index;

	index = 0;
	if (flag == INIT_MUTEX_ERROR)
		printf("Erorr occured during mutexes initialization\n Destroying Mutexes...\n");
	while (index < i)
	{
		if (pthread_mutex_destroy(&philos[index].right_fork) != 0)
			printf("Some error happened and the mutex is not destroyed successfully.\n");
		index++;
	}
	if (pthread_mutex_destroy(&philos->sync->state) != 0)
		printf("Some error happened and the mutex is not destroyed successfully.\n");
	return (NULL);
}

void	finalize_init(t_philo *philos, int total_philos)
{
	int	i;

	i = -1;
	while (++i < total_philos)
	{
		if (i == 0)
			philos[i].left_fork = &philos[total_philos - 1].right_fork;
		else
			philos[i].left_fork = &philos[i - 1].right_fork;
	}
	i = -1;
	while (++i < total_philos)
	{
		if (philos[i].philo_number % 2 == 0)
		{
			philos[i].first = philos[i].left_fork;
			philos[i].second = &philos[i].right_fork;
		}
		else
		{
			philos[i].first = &philos[i].right_fork;
			philos[i].second = philos[i].left_fork;
		}
	}
}

t_philo	*init_sync(t_sync *sync)
{
	int	i;
	t_philo	*philos;

	i = 0;
	philos = ft_malloc(sizeof(t_philo) * sync->total_philos, ALLOCATE, NULL, NULL);
	if (philos == NULL)
		return (NULL);
	memset(philos, 0x0, sizeof(t_philo) * sync->total_philos);
	if (pthread_mutex_init(&sync->state, NULL) != 0)
		return (NULL);
	if (pthread_mutex_init(&sync->write, NULL) != 0)
		return (NULL);
	if (pthread_mutex_init(&sync->checking, NULL) != 0)
		return (NULL);
	if (pthread_mutex_init(&sync->stopper, NULL) != 0)
		return (NULL);
	if (pthread_mutex_init(&sync->stats, NULL) != 0)
		return (NULL);
	while (i < sync->total_philos)
	{
		philos[i].philo_number = i + 1;
		philos[i].sync = sync;
		if (pthread_mutex_init(&philos[i].right_fork, NULL))
			return (destroy_mutexes(philos, i, INIT_MUTEX_ERROR));
		i++;
	}
	finalize_init(philos, sync->total_philos);
	return (philos);
}

void	test_print_forks(t_philo *philos, t_sync *args)
{
	int	i;

	i = 0;
	while (i < args->total_philos)
	{
		printf("[%p]<(%zu)>[%p]", philos[i].left_fork, philos[i].philo_number, &philos[i].right_fork);
		i++;
	}
}

size_t	timer_stamper(int flag)
{
	static size_t	start;
	struct timeval	current;

	gettimeofday(&current, NULL);
	if (flag)
		start = current.tv_sec * 1000 + current.tv_usec / 1000;
	return ((current.tv_sec * 1000 + current.tv_usec / 1000) - start);
}