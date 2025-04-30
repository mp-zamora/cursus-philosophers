/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 10:17:12 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/04/30 16:01:49 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	ft_error(char *err_msg, t_data *data)
{
	printf("Error: %s\n", err_msg);
	if (data)
		terminate_simulation(data);
	return (1);
}

void	free_philo_list(t_philo *list)
{
	t_philo	*aux;

	if (!list)
		return ;
	while (list)
	{
		if (list->philo_mutex)
		{
			pthread_mutex_destroy(list->philo_mutex);
			free (list->philo_mutex);
		}
		if (list->thread)
			free (list->thread);
		aux = list;
		list = list->next;
		free (aux);
	}
}

void	free_philo_forks(t_data *data)
{
	int	i;

	if (data->fork_mutex)
	{
		i = -1;
		while (++i < data->number_of_philos)
		{
			pthread_mutex_destroy(data->fork_mutex[i]);
			free (data->fork_mutex[i]);
		}
		free (data->fork_mutex);
		data->fork_mutex = NULL;
	}
	if (data->fork_status)
	{
		free (data->fork_status);
		data->fork_status = NULL;
	}
}

void	free_philo_data(t_data *data)
{
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
	free_philo_forks(data);
	free (data);
}

int	main(int argc, char *argv[])
{
	t_data	*data;

	if (argc != 5 && argc != 6)
		return (ft_error("Not enough args!", NULL));
	if (!ft_is_str_numeric(argv[1]) || !ft_is_str_numeric(argv[2])
		|| !ft_is_str_numeric(argv[3]) || !ft_is_str_numeric(argv[4]))
		return (ft_error("Only positive numeric inputs are allowed!", NULL));
	if (argc == 6 && !ft_is_str_numeric(argv[5]))
		return (ft_error("Only positive numeric inputs are allowed!", NULL));
	if (argv[1][0] == '0' || argv[2][0] == '0' || argv[3][0] == '0'
		|| argv[4][0] == '0' || (argc == 6 && argv[5][0] == '0'))
		return (ft_error("Arguments must not begin with zero!", NULL));
	if (initialize_philo_data(argc, argv, &data) != 0)
		return (ft_error("Initialization of philo data failed!", data));
	run_simulation(data);
	free_philo_data(data);
	return (EXIT_SUCCESS);
}
