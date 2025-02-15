/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 16:11:46 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/15 11:36:19 by soujaour         ###   ########.fr       */
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

void	list_add(t_alloc **head, t_alloc *new)
{
	if (new == NULL || head == NULL)
		return ;
	new->next = *head;
	*head = new;
}

void	*garbage_collector(t_alloc *allocs)
{
	t_alloc	*next;

	while (allocs)
	{
		next = allocs->next;
		free(allocs->content);
		free(allocs);
		allocs = next;
	}
	return (NULL);
}

void	*ft_malloc(size_t size, int flag, void *one, void *two)
{
	void			*ptr;
	t_alloc			*new;
	static t_alloc	*allocs;

	ptr = NULL;
	new = NULL;
	if (flag)
	{
		if (ptr)
			free(one);
		if (new)
			free(two);
		return (garbage_collector(allocs));
	}
	else
	{
		ptr = malloc(size);
		new = malloc(sizeof(t_alloc));
		if (ptr == NULL || new == NULL)
			ft_malloc(0, 1, ptr, new);
		new->content = ptr;
		new->next = NULL;
		list_add(&allocs, new);
	}
	return (ptr);
}