/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 10:17:12 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/04/09 00:37:33 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	ft_error(char *err_msg, t_data *data)
{
	printf("Error: %s\n", err_msg);
	terminate_simulation(data);
	return (1);
}

void	terminate_simulation(t_data *data)
{
	if (!data || !data->main_mutex)
		return ;
	pthread_mutex_lock(&(data->main_mutex));
	data->terminate = 1;
	pthread_mutex_unlock(&(data->main_mutex));
}

// REVISE FREES TO NEW VARIABLES IN STRUCTURES.
// Technically, with the new proposed archietchture, no MUTEX will be locked,
// and threads will be joined automatically on sim end.
void	free_philo_list(t_philo *list)
{
	t_philo	*aux;

	if (!list)
		return ;
	while (list)
	{
		if (list->fork_ids[0] != -1)
			if (pthread_mutex_unlock(list->data->forks[list->fork_ids[0]]) != 0)
				printf("Error: Failure unlocking mutex on termination.\n");
		if (list->fork_ids[1] != -1)
			if (pthread_mutex_unlock(list->data->forks[list->fork_ids[1]]) != 0)
				printf("Error: Failure unlocking mutex on termination.\n");
		if (list->thread)
		{
			if (pthread_join((*list->thread), NULL) != 0)
				printf("Error: Failure joining thread.\n");
			free (list->thread);
			list->thread = NULL;
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
	{
		free_philo_list(data->philo_list);
		data->philo_list = NULL;
	}
	if (data->main_mutex)
	{
		pthread_mutex_destroy(data->main_mutex);
		free (data->main_mutex);
		data->main_mutex = NULL;
	}
	if (data->fork_mutex)
	{
		i = -1;
		while (++i < data->number_of_philos)
		{
			pthread_mutex_destroy(data->forks[i]);
			free (data->forks[i]);
		}
		free (data->forks);
		data->forks = NULL;
	}
	free (data);
}

/* DEADLOCK FOUND -> valgrind --leak-check=full ./philo 5 410 200 100 */
int	main(int argc, char *argv[])
{
	t_data	*data;

	if (argc != 5 && argc != 6)
		ft_error("Not enough args!", NULL);
	if (initialize_philo_data(argc, argv, &data) != 0)
		return (EXIT_FAILURE);
	run_simulation(data);
	// launch_philo_threads(data);
	// if (monitor_philosophers(data) != 0)
	// 	return (EXIT_FAILURE);
	free_philo_data(data);
	return (EXIT_SUCCESS);
}
