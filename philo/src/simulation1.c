/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 12:05:12 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/05/03 13:46:31 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	run_simulation(t_data *data)
{
	t_philo	*iter;

	launch_threads(data);
	monitor_philosophers(data);
	iter = data->philo_list;
	while (iter)
	{
		if (pthread_join(*iter->thread, NULL) != 0)
			return (ft_error("Failure joining a thread.", data));
		iter = iter->next;
		custom_sleep(100, data);
	}
	return (0);
}

long	get_last_eat(t_philo *philo)
{
	long	last_eat_milis;

	pthread_mutex_lock(philo->philo_mutex);
	last_eat_milis = philo->last_eat_milis;
	pthread_mutex_unlock(philo->philo_mutex);
	return (last_eat_milis);
}

int	monitor_philosophers(t_data *data)
{
	t_philo	*iter;

	while (has_sim_started(data) != 1)
		custom_sleep(100, data);
	custom_sleep(5000, data);
	while (1)
	{
		iter = data->philo_list;
		while (iter)
		{
			if (get_current_ms(data) - get_last_eat(iter) >= data->time_to_die)
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
			custom_sleep(100, data);
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
	terminate_simulation(philo->data);
	pthread_mutex_lock(philo->data->write_mutex);
	printf("%ld %d died.\n", get_current_ms(philo->data)
		- philo->data->start_time, philo->number);
	pthread_mutex_unlock(philo->data->write_mutex);
	return (0);
}
