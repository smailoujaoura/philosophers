/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   before.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:37:01 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/16 16:39:58 by soujaour         ###   ########.fr       */
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
		if (pthread_mutex_destroy(&philos[index].right) != 0)
			printf("Some error happened and the mutex is not destroyed successfully.\n");
		index++;
	}
	if (pthread_mutex_destroy(&philos->sync->change) != 0)
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
			philos[i].left = &philos[total_philos - 1].right;
		}
		else
		{
			philos[i].left = &philos[i - 1].right;
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
	if (pthread_mutex_init(&synchro->change, NULL) != 0)
		return (NULL);
	while (i < args->total_philos)
	{
		philos[i].number = i + 1;
		philos[i].sync = synchro;
		philos[i].args = args;
		if (pthread_mutex_init(&philos[i].right, NULL) != 0)
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
		printf("[%p]<(%zu)>[%p]", philos[i].left, philos[i].number, &philos[i].right);
		i++;
	}
}

size_t	get_time(void)
{
	struct timeval	tv;
	size_t			current;

	gettimeofday(&tv, NULL);
	current = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (current);
}

# define DEATH 96
# define STOP 69

void	*start_routine(void *ptr)
{
	t_philo	*philos;
	philos = (t_philo *)ptr;

	pthread_mutex_lock(&philos->sync->change);
	philos->last = philos->sync->start;
	pthread_mutex_unlock(&philos->sync->change);
	while (1)
	{
		if (philos->meals && philos->meals == philos->args->cycles_total)
		{
			// printf("I AM STOPPING\n");
			return (NULL);
		}
		if (philos->number % 2 == 0)
		{
			pthread_mutex_lock(philos->left);
			printf("%zu ms %zu has taken left fork\n", get_time() - philos->sync->start, philos->number);
			pthread_mutex_lock(&philos->right);
			printf("%zu ms %zu has taken right fork\n", get_time() - philos->sync->start, philos->number);
		}
		else
		{
			pthread_mutex_lock(&philos->right);
			printf("%zu ms %zu has taken right fork\n", get_time() - philos->sync->start, philos->number);
			pthread_mutex_lock(philos->left);
			printf("%zu ms %zu has taken left fork\n", get_time() - philos->sync->start, philos->number);
		}

		// eating
		pthread_mutex_lock(&philos->sync->change);
		philos->last = get_time();
		pthread_mutex_unlock(&philos->sync->change);
		printf("%zu ms %zu is eating\n", get_time() - philos->sync->start, philos->number);
		philos->meals++;
		usleep(philos->args->eat_time);
		pthread_mutex_unlock(philos->left);
		pthread_mutex_unlock(&philos->right);

		// sleeping
		pthread_mutex_lock(&philos->sync->change);
		printf("%zu ms %zu is sleeping\n", get_time() - philos->sync->start, philos->number);
		pthread_mutex_unlock(&philos->sync->change);
		usleep(philos->args->sleep_time);

		// thinking
		pthread_mutex_lock(&philos->sync->change);
		printf("%zu ms %zu is thinking\n", get_time() - philos->sync->start, philos->number);
		pthread_mutex_unlock(&philos->sync->change);
	}
	return (NULL);
}

void	*start_sync(t_philo *philos, t_arg *args)
{
	size_t			i = 0;
	struct timeval	tv;

	// maybe lock the mutexes initally, but that would be make the threads run and consume the usleep wait time.
	gettimeofday(&tv, NULL);
	philos->sync->start = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	while (i < args->total_philos)
	{
		if (pthread_create(&philos[i].thread, NULL, start_routine, &philos[i]) != 0)
			return (NULL);
		i++;
	}
	i = 0;
	while (i < args->total_philos)
	{
		if (pthread_join(philos[i].thread, NULL) != 0)
			return (NULL);
		i++;
		if (i == args->total_philos)
			return (NULL);
	}
	int flag = 0;
	usleep(args->death_time);
	while (1)
	{
		i = 0;
		while (i < args->total_philos)
		{
			pthread_mutex_lock(&philos->sync->change);
			if (get_time() - philos[i].last >= args->death_time)
			{
				printf("%zu ms %zu died\n", get_time() - philos->sync->start, philos[i].number);
				flag = 1;
				pthread_mutex_unlock(&philos->sync->change);
				break ;
			}
			pthread_mutex_unlock(&philos->sync->change);
			i++;
		}
		if (flag)
			break ;
		usleep(100);
	}
	return ((void *)1);
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
			// test_print_forks(philos, args);
			start_sync(philos, args); // we create threads for each philo and then then 
			// pthread_create()
		}
	}
	else
		printf("Erroneous Arguments!\n");
	ft_malloc(0, 1, NULL, NULL);
	return (0);
}