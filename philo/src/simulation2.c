/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 12:07:06 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/02/06 12:26:46 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	kill_philosopher(t_philo *philo)
{
	long	current_milis;

	philo->status = -1;
	current_milis = get_current_milis(philo->data);
	printf("%ld %d died.\n", current_milis, philo->number);
	free_philo_data(philo->data);
	exit (EXIT_SUCCESS);
}

void	catch_forks(t_philo *philo)
{
	long	current_milis;

	if (philo->number == philo->data->number_of_philos)
	{
		if (pthread_mutex_lock(philo->data->forks[0]) != 0)
			ft_error("Failure locking mutex.\n", philo->data);
		current_milis = get_current_milis(philo->data);
		printf("%ld %d has taken a fork.\n", current_milis, philo->number);
	}
	if (pthread_mutex_lock(philo->data->forks[philo->number - 1]) != 0)
		ft_error("Failure locking mutex.\n", philo->data);
	current_milis = get_current_milis(philo->data);
	printf("%ld %d has taken a fork.\n", current_milis, philo->number);
	if (philo->number < philo->data->number_of_philos)
	{
		if (pthread_mutex_lock(philo->data->forks[philo->number]) != 0)
			ft_error("Failure locking mutex.\n", philo->data);
		current_milis = get_current_milis(philo->data);
		printf("%ld %d has taken a fork.\n", current_milis, philo->number);
	}
}

void	leave_forks(t_philo *philo)
{
	if (philo->number == philo->data->number_of_philos)
		if (pthread_mutex_unlock(philo->data->forks[0]) != 0)
			ft_error("Failure unlocking mutex.\n", philo->data);
	if (pthread_mutex_unlock(philo->data->forks[philo->number - 1]) != 0)
		ft_error("Failure unlocking mutex.\n", philo->data);
	if (philo->number < philo->data->number_of_philos)
		if (pthread_mutex_unlock(philo->data->forks[philo->number]) != 0)
			ft_error("Failure unlocking mutex.\n", philo->data);
}
