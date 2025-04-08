/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 12:07:06 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/04/09 00:32:14 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	kill_philosopher(t_philo *philo)
{
	long	current_milis;
	
	terminate_simulation(data);
	// pthread_mutex_lock(philo->data->main_mutex);
	// philo->data->terminate = 1;
	// pthread_mutex_unlock(philo->data->main_mutex);
	// pthread_mutex_lock(philo->philo_mutex);
	// philo->status = -1;
	// pthread_mutex_unlock(philo->philo_mutex);
	current_milis = get_current_milis(philo->data);
	printf("%ld %d died.\n", current_milis - philo->data->start_time,
		philo->number);
	return (0);
}

int	catch_first_fork(t_philo *philo)
{
	if (philo->fork_ids[0] == -1 && philo->status != -1)
	{
		if (philo->number == philo->data->number_of_philos)
		{
			if (pthread_mutex_lock(philo->data->forks[0]) != 0)
				return (ft_error("Failure locking mutex.\n", philo->data));
			pthread_mutex_lock(philo->data->main_mutex);
			philo->fork_ids[0] = 0;
			pthread_mutex_unlock(philo->data->main_mutex);
			printf("%ld %d has taken a fork [%d].\n", get_current_milis(philo->data) - philo->data->start_time,
				philo->number, 0);
		}
		else
		{
			if (pthread_mutex_lock(philo->data->forks[philo->number - 1]) != 0)
				return (ft_error("Failure locking mutex.\n", philo->data));
			pthread_mutex_lock(philo->data->main_mutex);
			philo->fork_ids[0] = philo->number - 1;
			pthread_mutex_unlock(philo->data->main_mutex);
			printf("%ld %d has taken a fork[%d].\n", get_current_milis(philo->data) - philo->data->start_time,
				philo->number, philo->number - 1);
		}
	}
	return (0);
}

int	catch_second_fork(t_philo *philo)
{
	if (philo->data->number_of_philos == 1 && philo->status != -1)
		return (1);
	if (philo->fork_ids[1] == -1)
	{
		if (philo->number == philo->data->number_of_philos)
		{
			if (pthread_mutex_lock(philo->data->forks[philo->number - 1]) != 0)
				return (ft_error("Failure locking mutex.\n", philo->data));
			pthread_mutex_lock(philo->data->main_mutex);
			philo->fork_ids[1] = philo->number - 1;
			pthread_mutex_unlock(philo->data->main_mutex);
			printf("%ld %d has taken a fork[%d].\n", get_current_milis(philo->data)\
				- philo->data->start_time, philo->number, philo->number - 1);
		}
		else
		{
			if (pthread_mutex_lock(philo->data->forks[philo->number]) != 0)
				return (ft_error("Failure locking mutex.\n", philo->data));
			pthread_mutex_lock(philo->data->main_mutex);
			philo->fork_ids[1] = philo->number;
			pthread_mutex_unlock(philo->data->main_mutex);
			printf("%ld %d has taken a fork[%d].\n", get_current_milis(philo->data)\
				- philo->data->start_time, philo->number, philo->number);
		}
	}
	return (0);
}

int	leave_forks(t_philo *philo)
{
	if (philo->fork_ids[0] != -1)
		if (pthread_mutex_unlock(philo->data->forks[philo->fork_ids[0]]) != 0)
			return (ft_error("Failure unlocking mutex.\n", philo->data));
	pthread_mutex_lock(philo->data->main_mutex);
	philo->fork_ids[0] = -1;
	pthread_mutex_unlock(philo->data->main_mutex);
	if (philo->fork_ids[1] != -1)
		if (pthread_mutex_unlock(philo->data->forks[philo->fork_ids[1]]) != 0)
			return (ft_error("Failure unlocking mutex.\n", philo->data));
	pthread_mutex_lock(philo->data->main_mutex);
	philo->fork_ids[1] = -1;
	pthread_mutex_unlock(philo->data->main_mutex);
	return (0);
}
