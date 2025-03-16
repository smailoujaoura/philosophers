/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 14:04:57 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/16 10:44:56 by soujaour         ###   ########.fr       */
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

void    ft_msleep(size_t millisecs)
{
    size_t    start_time;

    start_time = get_time();
	usleep((millisecs - 10) * 1000);
    while (millisecs + start_time > get_time())
        usleep(500);
}

// void ft_msleep(size_t millisecs)
// {
//     size_t start_time;
//     size_t elapsed;
//     size_t remaining;

//     start_time = get_time();
    
//     // Sleep for most of the time using usleep
//     usleep((millisecs * 950) / 1000);  // Sleep for 95% of the time
    
//     // Then use a short busy-wait for the remaining time for precision
//     elapsed = get_time() - start_time;
//     remaining = (elapsed < millisecs) ? (millisecs - elapsed) : 0;
    
//     while (remaining > 0)
//     {
//         usleep(100);  // Sleep in shorter increments
//         elapsed = get_time() - start_time;
//         remaining = (elapsed < millisecs) ? (millisecs - elapsed) : 0;
//     }
// }