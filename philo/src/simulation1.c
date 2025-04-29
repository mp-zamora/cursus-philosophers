/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 12:05:12 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/04/29 19:10:37 by mpenas-z         ###   ########.fr       */
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

long	get_last_eat_milis(t_philo *philo)
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
	long	last_eat_milis;
	long	current_milis;

	while (1)
	{
		iter = data->philo_list;
		while (iter)
		{
			last_eat_milis = get_last_eat_milis(iter);
			current_milis = get_current_milis(data);
			// printf("\033[32mCURRENT TIME: %ld\033[0m\n", current_milis - data->start_time);
			// printf("\033[33mLAST EAT TIME: %ld (%d)\033[0m\n", last_eat_milis - data->start_time, iter->number);
			// printf("\033[34mTIME ELAPSED: %ld\033[0m\n", current_milis - last_eat_milis);
			// printf("\033[35mTIME_TO_DIE: %ld\033[0m\n", data->time_to_die);
			if (current_milis - last_eat_milis > data->time_to_die)
			{
				// printf("\033[31mDEAD: %d\033[0m\n", iter->number);
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
	long	current_milis;

	terminate_simulation(philo->data);
	current_milis = get_current_milis(philo->data);
	printf("%ld %d died.\n", current_milis - philo->data->start_time,
		philo->number);
	return (0);
}
