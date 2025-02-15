/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:37:01 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/15 13:41:20 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_args	*check_args(char *argv[], int flag)
{
	int		error;
	t_args	*args;

	error = 0;
	args = ft_malloc(sizeof(t_args), 0, NULL, NULL);
	args->total_philos = custom_atoi(argv[1], &error);
	args->death_time = custom_atoi(argv[2], &error);
	args->eat_time = custom_atoi(argv[3], &error);
	args->sleep_time = custom_atoi(argv[4], &error);
	args->cycles_total = 0;
	if (flag)
		args->cycles_total = custom_atoi(argv[5], &error);
	if (error || !args->death_time || !args->eat_time || !args->sleep_time)
		return (NULL);
	return (args);
}

// printf("Erroneous Arguments!\n");
int	main(int argc, char *argv[])
{
	t_args	*args;
	
	if (argc == 5 || argc == 6)
	{
		args = check_args(argv, argc == 6);
		if (args == NULL || (argc == 6 && args->cycles_total == 0))
			return (1);
		else if (args->total_philos > TOTAL_PHILOS)
			printf("Maximum number of philos is 200\n");
		else
		{
			printf("args: %ld %ld %ld %ld %ld\n", args->total_philos, args->death_time, args->eat_time, args->sleep_time, args->cycles_total);
			// init_sync();
			// start_sync();
		}
	}
	else
		printf("Erroneous Arguments!\n");
	ft_malloc(0, 1, NULL, NULL);
	return (0);
}