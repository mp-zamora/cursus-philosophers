/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 12:07:06 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/02/22 18:11:30 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	kill_philosopher(t_philo *philo)
{
	long	current_milis;

	philo->status = -1;
	philo->data->terminate = 1;
	current_milis = get_current_milis(philo->data);
	printf("%ld %d died.\n", current_milis, philo->number);
	free_philo_data(philo->data);
	exit (EXIT_SUCCESS);
}

void	catch_first_fork(t_philo *philo)
{
	if (philo->fork_ids[0] == -1)
	{
		if (philo->number == philo->data->number_of_philos)
		{
			if (pthread_mutex_lock(philo->data->forks[0]) != 0)
				ft_error("Failure locking mutex.\n", philo->data);
			philo->fork_ids[0] = 0;
			printf("%ld %d has taken a fork.\n", get_current_milis(philo->data) - philo->data->start_time,
				philo->number);
		}
		else
		{
			if (pthread_mutex_lock(philo->data->forks[philo->number - 1]) != 0)
				ft_error("Failure locking mutex.\n", philo->data);
			philo->fork_ids[0] = philo->number - 1;
			printf("%ld %d has taken a fork.\n", get_current_milis(philo->data) - philo->data->start_time,
				philo->number);
		}
	}
}

void	catch_second_fork(t_philo *philo)
{
	if (philo->data->number_of_philos == 1)
		return ;
	if (philo->fork_ids[1] == -1)
	{
		if (philo->number == philo->data->number_of_philos)
		{
			if (pthread_mutex_lock(philo->data->forks[philo->number - 1]) != 0)
				ft_error("Failure locking mutex.\n", philo->data);
			philo->fork_ids[1] = philo->number - 1;
			printf("%ld %d has taken a fork.\n", get_current_milis(philo->data) - philo->data->start_time,
				philo->number);
		}
		else
		{
			if (pthread_mutex_lock(philo->data->forks[philo->number]) != 0)
				ft_error("Failure locking mutex.\n", philo->data);
			philo->fork_ids[1] = philo->number;
			printf("%ld %d has taken a fork.\n", get_current_milis(philo->data) - philo->data->start_time,
				philo->number);
		}
	}
}

void	leave_forks(t_philo *philo)
{
	if (philo->fork_ids[0] != -1)
		if (pthread_mutex_unlock(philo->data->forks[philo->fork_ids[0]]) != 0)
			ft_error("Failure unlocking mutex.\n", philo->data);
	philo->fork_ids[0] = -1;
	if (philo->fork_ids[1] != -1)
		if (pthread_mutex_unlock(philo->data->forks[philo->fork_ids[1]]) != 0)
			ft_error("Failure unlocking mutex.\n", philo->data);
	philo->fork_ids[1] = -1;
}
