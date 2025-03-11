/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 16:11:46 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/09 12:44:41 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	custom_atoi(const char *str, int *error)
{
	size_t	num;

	num = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			*error = 1;
		str++;
	}
	if (!*str)
		*error = 1;
	while (*str)
	{
		if (num > SIZE_MAX / 10
			|| (num == SIZE_MAX / 10 && (size_t)(*str - '0') > SIZE_MAX % 10))
			*error = 1;
		if (*str >= '0' && *str <= '9')
			num = num * 10 + (*str - '0');
		else
			*error = 1;
		str++;
	}
	return (num);
}
