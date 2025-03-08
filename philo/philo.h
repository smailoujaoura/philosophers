/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:38:02 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/08 22:14:52 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

// Might need to remove some unsued headers at the end of the project.
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdint.h>
# include <limits.h>

# define TOTAL_PHILOS 200
# define ALLOCATE 1337
# define DEALLOCATE 42
# define INIT_MUTEX_ERROR 22

typedef struct s_alloc
{
	void			*content;
	struct s_alloc	*next;
}	t_alloc;

typedef struct s_sync
{
	int				total_philos;
	ssize_t			cycles_total;
	size_t			death_time;
	size_t			eat_time;
	size_t			sleep_time;
	pthread_mutex_t	state;
	pthread_mutex_t write;
	pthread_mutex_t checking;
	pthread_mutex_t	stopper;
	pthread_mutex_t	stats;
	int				stop;
	size_t			starting_time;
}	t_sync;

typedef struct s_philo
{
	size_t			philo_number;
	pthread_t		thread_id;
	size_t			last_meal;
	ssize_t			total_meals;
	pthread_mutex_t	right_fork;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;
	t_sync			*sync;
}	t_philo;


void	*ft_malloc(size_t size, int flag, void *one, void *two);
size_t	custom_atoi(const char *str, int *error);


// 
t_philo	*init_sync(t_sync *sync);
void	finalize_init(t_philo *philos, int total_philos);
void	*destroy_mutexes(t_philo *philos, size_t i, int flag);
void	precise_sleep(size_t micro_secs);
size_t	timer_stamper(int flag);
void    ft_msleep(size_t millisec);
t_sync	*check_args(char *argv[], int flag);
size_t	get_time(void);
int		safe_writing_messages(t_philo *philo, char *message, int f);


#endif