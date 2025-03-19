/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:38:02 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/19 10:43:06 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <semaphore.h>
# include <string.h>
# include <pthread.h>
# include <stdbool.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <stdint.h>

# define DESTROY -1
# define INITIALIZE 1

# define BINARY 1
# define SERIALIZER 0

# define MULTIPLIER 1664525
# define INCREMENT 1013904223
# define MODULUS 62

typedef struct s_philo	t_philo;

typedef struct s_sync
{
	int				eat_time;
	int				sleep_time;
	int				total_philos;
	int				total_cycles;
	size_t			death_time;
	pid_t			pid;
	pid_t			*pids;
	char			name[5][21];
	size_t			start_time;
	sem_t			*write_sem;
	sem_t			*meals_sem;
	sem_t			*death_sem;
	sem_t			*forks_sem;
	sem_t			*resources_sem;
	t_philo			*philos_ptr;
	pthread_t		monitor_tid;
}	t_sync;

typedef struct s_philo
{
	int				number;
	int				total;
	size_t			last;
	t_sync			*sync;
}	t_philo;

void	ft_msleep(size_t millisecs);
size_t	get_time(void);
size_t	custom_atoi(char *s, int *err);
int		check_args(t_sync *sync, char **argv, int optional);
int		init_sems(t_sync *sync, int num_philo);
int		init_sync(t_sync *sync, t_philo **ptr);
char	*random_name(char *name);
void	philosopher(t_philo *philo);

#endif