/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rand_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 10:09:01 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/19 11:51:47 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

char	*random_name(char *name)
{
	static size_t	seed;
	size_t			key;
	int				i;

	i = 0;
	name[i] = '/';
	if (seed == 0)
		seed = get_time();
	else
		seed += get_time();
	while (++i < 20)
	{
		key = (seed * MULTIPLIER + INCREMENT) % MODULUS;
		if (key < 26)
			name[i] = 'A' + key % 26;
		else if (key >= 26 && key < 52)
			name[i] = 'a' + key % 26;
		else
			name[i] = '0' + key % 10;
		seed *= MULTIPLIER;
	}
	name[i] = '\0';
	return (name);
}
