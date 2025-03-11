/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:38:02 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/11 16:13:20 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <semaphore.h>
# include <string.h>
# include <pthread.h>
# include <stdint.h>
# include <sys/time.h>
# include <sys/wait.h>

# define BINARY 1
# define SERIALIZER 0

typedef struct s_sync
{
	int				total_philos;
	int				total_cycles;
	int				eat_time;
	int				sleep_time;
	int				stop;
	size_t			start_time;
	size_t			death_time;
	sem_t			*forks;
	sem_t			*logs;
	sem_t			*checks;
	char			array[3][21];
}	t_sync;

typedef struct s_philo
{
	int				total;
	int				number;
	pid_t			pid;
	size_t			last;
	t_sync			*sync;
	// pthread_t		thread;
	// pthread_mutex_t	*right;
	// pthread_mutex_t	*left;
	// pthread_mutex_t	*first;
	// pthread_mutex_t	*second;
}	t_philo;

int		check_args(t_sync *sync, char **argv, int optional);
int		init_everything(t_sync *sync, t_philo *philos, int total, int i);
void    ft_msleep(size_t millisec);
size_t	get_time(void);
void	close_and_free(t_sync *sync, t_philo *philos);
size_t	custom_atoi(const char *str, int *error);

#endif