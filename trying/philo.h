/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:38:02 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/16 18:09:16 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>
# include <stdint.h>
# include <sys/time.h>
# include <stdbool.h>

# define DIED_MESSAGE "died"
# define EATING_MESSAGE "is eating" 
# define THINKING_MESSAGE "is thinking"
# define SLEEPING_MESSAGE "is sleeping"
# define FORK_MESSAGE "has taken a fork"

# define DESTROY -1
# define INITIALIZE 1

typedef struct s_philo t_philo;

typedef struct s_sync
{
	int				eat_time;
	int				sleep_time;
	int				total_philos;
	int				total_cycles;
	size_t			death_time;
	size_t			start_time;
	pthread_t		monitor_tpid;
	pthread_mutex_t write_mutex;
	pthread_mutex_t	meals_mutex;
	pthread_mutex_t death_mutex;
	pthread_mutex_t end_mutex;
	t_philo			*philos_ptr;
}	t_sync;

typedef struct s_philo
{
	int				total;
	int				number;
	t_sync			*sync;
	size_t			last;
	pthread_t		tid;
	pthread_mutex_t	right;
	pthread_mutex_t	*left;

	pthread_mutex_t	*first;
	pthread_mutex_t	*second;
}	t_philo;

// HOT 
int		check_args(t_sync *sync, char **argv, int optional);
int		init_mutexes(t_sync *sync, int action, int *array);
int		init_sync(t_sync *sync, t_philo **ptr, int i);
bool	end_simulation(int flag, t_sync *sync);
void	destroy_and_free(t_sync *sync);

// COLD
void	ft_msleep(size_t millisec);
size_t	get_time(void);
size_t	custom_atoi(const char *str, int *error);

#endif