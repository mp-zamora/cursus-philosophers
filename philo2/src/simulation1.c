/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 12:05:12 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/04/09 11:40:39 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	run_simulation(t_data *data)
{
	launch_threads(data);
	monitor_philosophers(data);
	join_threads(data);
	return (0);
}

long  get_last_eat_milis(t_philo *philo)
{
	long  last_eat_milis;

	pthread_mutex_lock(philo->philo_mutex);
	last_eat_milis = philo->last_eat_milis;
	pthread_mutex_unlock(philo->philo_mutex);
	return (last_eat_milis);
}

int	monitor_philosophers(t_data *data)
{
	t_philo		*iter;

	while (1)
	{
		iter = data->philo_list;
		while (iter)
		{
			if (get_current_milis(data) - get_last_eat_milis(iter) \
				> data->time_to_die)
			{
				kill_philosopher(iter);
				break ;
			}
			iter = iter->next;
		}
		if (did_philosophers_eat_enough(data) == TRUE)
			terminate_simulation(data);
		if (check_termination(data))
			break ;
		else
			usleep(5000);
	}
	return (0);
}

int	check_termination(t_data *data)
{
	int	terminate;

	pthread_mutex_lock(data->main_mutex);
	terminate = data->terminate;
	pthread_mutex_unlock(data->main_mutex);
	return (terminate);
}

int	kill_philosopher(t_philo *philo)
{
	long	current_milis;
	
	terminate_simulation(philo->data);
	current_milis = get_current_milis(philo->data);
	printf("%ld %d died.\n", current_milis - philo->data->start_time,
		philo->number);
	return (0);
}

