/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:37:01 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/18 21:17:29 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
	kill(sync->pid, SIGTERM);
	return (NULL);
}

int	clean_exit(t_sync *sync)
{
	int	i;
	int	status;

	waitpid(-1, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGTERM)
	{
		i = 0;
		while (++i < sync->total_philos)
			kill(sync->pids[i], SIGTERM);
	}
	else
	{
		i = -1;
		while (++i < sync->total_philos)
			kill(sync->pids[i], SIGTERM);
	}
	while (waitpid(-1, NULL, 0) > 0)
		;
	return (0);
}

int	start_sync(t_philo *philos, t_sync *sync, int i)
{
	sync->start_time = get_time();
	while (++i < sync->total_philos)
	{
		sync->pids[i] = fork();
		if (i == 0)
			sync->pid = sync->pids[i];
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
	if (sync->total_cycles > 0)
	{
		if (pthread_create(&sync->monitor_tid, NULL, count_meals, sync))
			return (-1);
		pthread_detach(sync->monitor_tid);
	}
	return (clean_exit(sync));
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
