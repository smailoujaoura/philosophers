/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:38:02 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/18 14:44:09 by soujaour         ###   ########.fr       */
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

typedef struct s_philo	t_philo;

typedef struct s_sync
{
	int				eat_time;
	int				sleep_time;
	int				total_philos;
	int				total_cycles;
	
	size_t			death_time;
	size_t			start_time;

	sem_t			*write_sem;
	sem_t			*meals_sem;
	sem_t			*death_sem;
	sem_t			*forks_sem;

	pid_t			*pids;
	pthread_t		monitor;

	char			name[4][21];

	t_philo			*philos_ptr;
}	t_sync;

typedef struct s_philo
{
	int				number;
	int				total;
	size_t			last;
	t_sync			*sync;

	
}	t_philo;

size_t	get_time(void);
size_t	custom_atoi(char *s, int *err);
void	ft_msleep(size_t millisecs /*t_sync *sync */);
int		check_args(t_sync *sync, char **argv, int optional);

int		init_sems(t_sync *sync, int num_philo);
int		init_sync(t_sync *sync, t_philo **ptr);




char	*prng(char *name);

#endif