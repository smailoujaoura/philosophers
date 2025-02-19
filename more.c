/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:56:55 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/19 12:43:22 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_arg	*check_args(char *argv[], int flag)
{
	int		error;
	t_arg	*args;

	error = 0;
	args = ft_malloc(sizeof(t_arg), 0, NULL, NULL);
	args->total_philos = custom_atoi(argv[1], &error);
	args->death_time = custom_atoi(argv[2], &error) * 1000;
	args->eat_time = custom_atoi(argv[3], &error) * 1000;
	args->sleep_time = custom_atoi(argv[4], &error) * 1000;
	args->cycles_total = 0;
	if (flag)
		args->cycles_total = custom_atoi(argv[5], &error);
	if (error || !args->death_time || !args->eat_time || !args->sleep_time)
		return (NULL);
	return (args);
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
	if (pthread_mutex_destroy(&philos->sync->status_change) != 0)
		printf("Some error happened and the mutex is not destroyed successfully.\n");
	return (NULL);
}

void	finalize_init(t_philo *philos, size_t total_philos)
{
	size_t	i;

	i = 0;
	while (i < philos->args->total_philos)
	{
		if (i == 0)
		{
			philos[i].left_fork = &philos[total_philos - 1].right_fork;
		}
		else
		{
			philos[i].left_fork = &philos[i - 1].right_fork;
		}
		i++;
	}
}

t_philo	*init_sync(t_arg *args)
{
	size_t	i;
	t_philo	*philos;
	t_sync	*synchro;

	i = 0;
	philos = ft_malloc(sizeof(t_philo) * args->total_philos, ALLOCATE, NULL, NULL);
	memset(philos, 0x0, sizeof(t_philo) * args->total_philos);
	synchro = ft_malloc(sizeof(t_sync), ALLOCATE, NULL, NULL);
	memset(synchro, 0x0, sizeof(t_sync));
	if (philos == NULL || synchro == NULL)
		return (NULL);
	if (pthread_mutex_init(&synchro->status_change, NULL) != 0)
		return (NULL);
	while (i < args->total_philos)
	{
		philos[i].philo_number = i + 1;
		philos[i].sync = synchro;
		philos[i].args = args;
		if (pthread_mutex_init(&philos[i].right_fork, NULL) != 0)
			return (destroy_mutexes(philos, i, INIT_MUTEX_ERROR));
		i++;
	}
	finalize_init(philos, args->total_philos);
	return (philos);
}

void	test_print_forks(t_philo *philos, t_arg *args)
{
	
	size_t	i = 0;
	while (i < args->total_philos)
	{
		printf("[%p]<(%zu)>[%p]", philos[i].left_fork, philos[i].philo_number, &philos[i].right_fork);
		i++;
	}
}

void precise_sleep(size_t micro_secs)
{
	size_t			remainder;
	size_t			elapsed;
	struct timeval	current;
	struct timeval	start;

	gettimeofday(&start, NULL);
	elapsed = 0;
	while (elapsed < micro_secs)
	{
		gettimeofday(&current, NULL);
		elapsed = (current.tv_sec - start.tv_sec) * 1000000 + (current.tv_usec - start.tv_usec);
		remainder = micro_secs - elapsed;

		if (remainder > 3000) 
			usleep(remainder / 2);
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