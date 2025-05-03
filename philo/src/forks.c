/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 10:54:42 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/05/03 14:35:22 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	catch_first_fork(t_philo *philo)
{
	int	target_fork;

	if (philo->number == 1)
		target_fork = philo->data->number_of_philos - 1;
	else
		target_fork = philo->number - 2;
	pthread_mutex_lock(philo->data->fork_mutex[target_fork]);
	philo->fork_ids[0] = target_fork;
	if (!check_termination(philo->data))
	{
		pthread_mutex_lock(philo->data->write_mutex);
		printf("%ld %d has taken a fork.\n", \
			get_current_ms(philo->data) - philo->data->start_time,
			philo->number);
		pthread_mutex_unlock(philo->data->write_mutex);
	}
	return (0);
}

int	catch_second_fork(t_philo *philo)
{
	int	target_fork;

	target_fork = philo->number - 1;
	pthread_mutex_lock(philo->data->fork_mutex[target_fork]);
	philo->fork_ids[1] = target_fork;
	if (!check_termination(philo->data))
	{
		pthread_mutex_lock(philo->data->write_mutex);
		printf("%ld %d has taken a fork.\n", \
			get_current_ms(philo->data) - philo->data->start_time,
			philo->number);
		pthread_mutex_unlock(philo->data->write_mutex);
	}
	return (0);
}

int	leave_forks(t_philo *philo)
{
	if (philo->fork_ids[1] != -1)
	{
		pthread_mutex_unlock(philo->data->fork_mutex[philo->fork_ids[1]]);
		philo->fork_ids[1] = -1;
	}
	if (philo->fork_ids[0] != -1)
	{
		pthread_mutex_unlock(philo->data->fork_mutex[philo->fork_ids[0]]);
		philo->fork_ids[0] = -1;
	}
	return (0);
}

int	has_first_fork(t_philo *philo)
{
	int	has_fork;

	has_fork = 0;
	if (philo->fork_ids[0] == -1)
		has_fork = 0;
	else
		has_fork = 1;
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
