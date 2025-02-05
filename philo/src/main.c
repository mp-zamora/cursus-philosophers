/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 10:17:12 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/02/05 12:34:09 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	monitor_philosophers(t_data *data)
{
	(void)data;
	return (1);
}

int	launch_philo_threads(t_data *data)
{
	(void)data;
	return (1);
}

int	run_simulation(t_data *data)
{
	int	exit_status;

	exit_status = launch_philo_threads(data);
	if (exit_status == 0)
		exit_status = monitor_philosophers(data);
	return (exit_status);
}

void	ft_error(char *err_msg, t_data *data)
{
	ft_putstr_fd("Error: ", 2);
	ft_putendl_fd(err_msg, 2);
	free_philo_data(data);
	exit (EXIT_FAILURE);
}

int	main(int argc, char *argv[])
{
	t_data	*data;
	int		exit_status;

	if (argc != 5 && argc != 6)
		ft_error("Not enough args!", NULL);
	exit_status = initialize_philo_data(argc, argv, &data);
	if (exit_status != SUCCESS)
		ft_error("Failure initializing the data structure.", data);
	exit_status = run_simulation(data);
	free_philo_data(data);
	return (exit_status);
}
