/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:38:02 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/17 12:15:10 by soujaour         ###   ########.fr       */
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

# define DESTROY -1
# define INITIALIZE 1

typedef struct s_philo	t_philo;

typedef struct s_sync
{
	int				eat_time;
	int				sleep_time;
	int				total_philos;
	int				total_cycles;
	size_t			death_time;
	size_t			start_time;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	meals_mutex;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	end_mutex;
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

size_t	get_time(void);
size_t	custom_atoi(char *s, int *err);
void	destroy_and_free(t_sync *sync);
void	ft_msleep(size_t millisecs, t_sync *sync);
bool	end_simulation(int flag, t_sync *sync);
int		check_args(t_sync *sync, char **argv, int optional);
int		init_mutexes(t_sync *sync, int action, int *array);
int		init_sync(t_sync *sync, t_philo **ptr, int i);
void	*philosopher(void *ptr);

#endif