/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 14:04:57 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/18 13:52:51 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

size_t	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (0);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_msleep(size_t millisecs /*t_sync *sync */)
{
	size_t	start_time;

	start_time = get_time();
	usleep(millisecs * 0.7 * 1000);
	while (millisecs + start_time > get_time())
	{
		// if (end_simulation(0, sync))
		// 	break ;
		usleep(500);
	}
}


// size_t	get_time(void)
// {
// 	struct timeval	tv;

// 	gettimeofday(&tv, NULL);
// 	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
// }

// void    ft_msleep(size_t millisec)
// {
//     size_t    start_time;

//     start_time = get_time();
// 	usleep((millisec - 10) * 1000);
//     while (millisec + start_time > get_time())
//         usleep(50);
// }