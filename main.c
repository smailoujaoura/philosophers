/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:37:01 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/08 16:19:17 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_data	*check_args(char *argv[], int flag)
{
	int		i;
	size_t	num;
	size_t	error;
	t_data	*args;

	i = 1;
	error = 0;
	args = ft_malloc(sizeof(t_data), 0, NULL, NULL);
	args->total_philos = custom_atoi(argv[1], &error);
	args->death_time = custom_atoi(argv[1], &error);
	args->eating_time = custom_atoi(argv[1], &error);
	args->sleeping_time = custom_atoi(argv[1], &error);
	if (flag)
		args->total_philos = custom_atoi(argv[1], &error);
	if (error)
		return (NULL);
	return (args);
}

int	main(int argc, char *argv[])
{
	t_data	*args;
	t_sync	*sync;
	
	if (argc == 5 || argc == 6)
	{
		args = check_args(argv, argc == 6);
		if (args == NULL)
			printf("Erroneous Arguments!\n");
		else
		{
			init_sync();
			start_sync();
		}
	}
	else
		printf("Erroneous Arguments!\n");
	ft_malloc(0, 1, NULL, NULL);
	return (0);
}