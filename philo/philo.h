/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:38:02 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/09 13:54:40 by soujaour         ###   ########.fr       */
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

# define MAX_PHILOS 200

typedef struct s_sync
{
	int				total_philos;
	int				total_cycles;
	size_t			death_time;
	int				eat_time;
	int				sleep_time;
	size_t			start_time;
	pthread_mutex_t *writing_mutex;
	pthread_mutex_t *checking_mutex;
}	t_sync;

typedef struct s_philo
{
	int				total;
	int				number;
	t_sync			*sync;
	size_t			last;
	pthread_t		thread;
	pthread_mutex_t	*right;
	pthread_mutex_t	*left;
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;
}	t_philo;

// t_sync	*check_args(char *argv[], int optional);
int		check_args(t_sync *sync, char *argv[], int optional);
// t_philo	*init_sync(t_sync *sync);
int		init_sync(t_sync *sync, t_philo *philos);

void	ft_msleep(size_t millisec);
size_t	get_time(void);
size_t	custom_atoi(const char *str, int *error);

#endif