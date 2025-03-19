/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:09:06 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/19 15:57:32 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	custom_atoi(char *s, int *err)
{
	size_t	num;

	num = 0;
	while ((*s >= 9 && *s <= 13) || *s == 32)
		s++;
	if (*s == '-' || *s == '+')
	{
		if (*s == '-')
			*err = 1;
		s++;
	}
	if (!*s)
		*err = 1;
	while (*s)
	{
		if (num > SIZE_MAX / 10
			|| (num == SIZE_MAX / 10 && (size_t)(*s - '0') > SIZE_MAX % 10))
			*err = 1;
		if (*s >= '0' && *s <= '9')
			num = num * 10 + (*s - '0');
		else
			*err = 1;
		s++;
	}
	return (num);
}
