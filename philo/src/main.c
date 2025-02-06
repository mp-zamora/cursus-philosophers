/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 10:17:12 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/02/06 14:14:52 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	ft_error(char *err_msg, t_data *data)
{
	printf("Error: %s\n", err_msg);
	free_philo_data(data);
	exit (EXIT_FAILURE);
}

void	free_philo_list(t_philo *list)
{
	t_philo	*aux;

	if (!list)
		return ;
	while (list)
	{
		if (list->thread)
		{
			if (pthread_join((*list->thread), NULL) != 0)
				printf("Error: Failure joining thread.\n");
			free (list->thread);
		}
		aux = list;
		list = list->next;
		free (aux);
	}
}

void	free_philo_data(t_data *data)
{
	int	i;

	if (!data)
		return ;
	if (data->philo_list)
		free_philo_list(data->philo_list);
	if (data->forks)
	{
		i = -1;
		while (data->forks[++i])
		{
			if (pthread_mutex_destroy(data->forks[i]) != 0)
				printf("Error: Failure destroying mutex.\n");
			free (data->forks[i]);
		}
		free (data->forks);
	}
	free (data);
}

int	main(int argc, char *argv[])
{
	t_data	*data;

	if (argc != 5 && argc != 6)
		ft_error("Not enough args!", NULL);
	initialize_philo_data(argc, argv, &data);
	launch_philo_threads(data);
	monitor_philosophers(data);
	free_philo_data(data);
	return (EXIT_SUCCESS);
}
