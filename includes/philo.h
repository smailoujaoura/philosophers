/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:21:12 by hel-makh          #+#    #+#             */
/*   Updated: 2022/01/31 19:21:12 by hel-makh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>

typedef struct s_args {
	int				philo_count;
	long			die_time;
	long			eat_time;
	long			sleep_time;
	int				meals_count;
}	t_args;

typedef struct s_vars {
	t_args			args;
	pthread_mutex_t	state_change;
	long			simulation_start;
	int				simulation_ended;
}	t_vars;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	right_fork;
	pthread_mutex_t	*left_fork;
	long			last_meal;
	int				meals_count;
	t_vars			*vars;
}	t_philo;

void	*ft_calloc(size_t count, size_t size);
void	*ft_free(void *ptr);
void	*ft_free_2d(void **ptr);
void	*ft_free_3d(void ***ptr);
long	ft_get_current_time(t_vars *vars);
void	ft_msleep(long sleeptime);

int		ft_parse_args(int argc, char *argv[], t_args *args);
t_philo	*ft_init_philos(t_vars *vars);
int		ft_init_mutexes(t_philo *philo);
void	ft_destroy_mutexes(t_philo *philo);
int		ft_meals_count_reached(t_philo *philo);
void	ft_print_state(char *state, t_philo *philo);

#endif