/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 10:54:42 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/04/30 15:59:16 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	catch_first_fork(t_philo *philo)
{
	int	target_fork;

	if (philo->number % 2 == 0)
		target_fork = philo->number % philo->data->number_of_philos;
	else
		target_fork = philo->number - 1;
	pthread_mutex_lock(philo->data->fork_mutex[target_fork]);
	if (philo->data->fork_status[target_fork] == 0)
	{
		philo->data->fork_status[target_fork] = 1;
		pthread_mutex_lock(philo->philo_mutex);
		philo->fork_ids[0] = target_fork;
		pthread_mutex_unlock(philo->philo_mutex);
	}
	else
	{
		pthread_mutex_unlock(philo->data->fork_mutex[target_fork]);
		return (0);
	}
	pthread_mutex_unlock(philo->data->fork_mutex[target_fork]);
	printf("%ld %d has taken a fork.\n", \
		get_current_ms(philo->data) - philo->data->start_time, philo->number);
	return (0);
}

int	catch_second_fork(t_philo *philo)
{
	int	target_fork;

	if (philo->number % 2 == 0)
		target_fork = philo->number - 1;
	else
		target_fork = philo->number % philo->data->number_of_philos;
	pthread_mutex_lock(philo->data->fork_mutex[target_fork]);
	if (philo->data->fork_status[target_fork] == 0)
	{
		philo->data->fork_status[target_fork] = 1;
		pthread_mutex_lock(philo->philo_mutex);
		philo->fork_ids[1] = target_fork;
		pthread_mutex_unlock(philo->philo_mutex);
	}
	else
	{
		pthread_mutex_unlock(philo->data->fork_mutex[target_fork]);
		return (0);
	}
	pthread_mutex_unlock(philo->data->fork_mutex[target_fork]);
	printf("%ld %d has taken a fork.\n", \
		get_current_ms(philo->data) - philo->data->start_time, philo->number);
	return (0);
}

int	leave_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->philo_mutex);
	pthread_mutex_lock(philo->data->fork_mutex[philo->fork_ids[0]]);
	philo->data->fork_status[philo->fork_ids[0]] = 0;
	pthread_mutex_unlock(philo->data->fork_mutex[philo->fork_ids[0]]);
	philo->fork_ids[0] = -1;
	pthread_mutex_lock(philo->data->fork_mutex[philo->fork_ids[1]]);
	philo->data->fork_status[philo->fork_ids[1]] = 0;
	pthread_mutex_unlock(philo->data->fork_mutex[philo->fork_ids[1]]);
	philo->fork_ids[1] = -1;
	pthread_mutex_unlock(philo->philo_mutex);
	return (0);
}

int	has_first_fork(t_philo *philo)
{
	int	has_fork;

	has_fork = 0;
	pthread_mutex_lock(philo->philo_mutex);
	if (philo->fork_ids[0] == -1)
		has_fork = 0;
	else
		has_fork = 1;
	pthread_mutex_unlock(philo->philo_mutex);
	return (has_fork);
}

int	has_second_fork(t_philo *philo)
{
	int	has_fork;

	has_fork = 0;
	pthread_mutex_lock(philo->philo_mutex);
	if (philo->fork_ids[1] == -1)
		has_fork = 0;
	else
		has_fork = 1;
	pthread_mutex_unlock(philo->philo_mutex);
	return (has_fork);
}
