/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 14:04:57 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/17 12:17:47 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (0);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_msleep(size_t millisecs, t_sync *sync)
{
	size_t	start_time;

	start_time = get_time();
	while (millisecs + start_time > get_time())
	{
		if (end_simulation(0, sync))
			break ;
		usleep(500);
	}
}
