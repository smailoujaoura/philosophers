/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:38:02 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/08 16:18:38 by soujaour         ###   ########.fr       */
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

typedef struct s_alloc
{
	void			*content;
	struct s_alloc	*next;
}	t_alloc;

typedef struct s_data
{
	size_t	total_philos;
	size_t	death_time;
	size_t	eating_time;
	size_t	sleeping_time;
	size_t	cycles_total;
}	t_data;


typedef struct s_sync
{
	int k;
	t_data	*data;
}	t_sync;

void	*ft_malloc(size_t size, int flag, void *one, void *two);

#endif