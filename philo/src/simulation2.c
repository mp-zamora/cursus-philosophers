/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 12:07:06 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/04/10 12:34:58 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	go_think(t_philo *philo)
{
	long	current_milis;

	pthread_mutex_lock(philo->philo_mutex);
	philo->status = 1;
	current_milis = get_current_milis(philo->data);
	philo->last_change_milis = current_milis;
	pthread_mutex_unlock(philo->philo_mutex);
	printf("%ld %d is thinking.\n", \
		current_milis - philo->data->start_time, philo->number);
	while (1)
	{
		if (!has_first_fork(philo))
			catch_first_fork(philo);
		if (!has_second_fork(philo))
			catch_second_fork(philo);
		if (check_termination(philo->data) ||
			(has_first_fork(philo) && has_second_fork(philo)))
			break ;
	}
	return (0);
}

int	go_eat(t_philo *philo)
{
	long	current_milis;

	pthread_mutex_lock(philo->philo_mutex);
	philo->status = 2;
	current_milis = get_current_milis(philo->data);
	philo->last_change_milis = current_milis;
	philo->last_eat_milis = current_milis;
	pthread_mutex_unlock(philo->philo_mutex);
	printf("%ld %d is eating.\n", \
		current_milis - philo->data->start_time, philo->number);
	usleep(philo->data->time_to_eat * 1000);
	pthread_mutex_lock(philo->philo_mutex);
	philo->times_eaten += 1;
	pthread_mutex_unlock(philo->philo_mutex);
	leave_forks(philo);
	return (0);
}

int	go_sleep(t_philo *philo)
{
	long  current_milis;

	pthread_mutex_lock(philo->philo_mutex);
	philo->status = 0;
	current_milis = get_current_milis(philo->data);
	philo->last_change_milis = current_milis;
	pthread_mutex_unlock(philo->philo_mutex);
	printf("%ld %d is sleeping.\n", \
		current_milis - philo->data->start_time, philo->number);
	usleep(philo->data->time_to_eat * 1000);
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (check_termination(philo->data))
			break ;
		go_think(philo);
		if (check_termination(philo->data))
			break ;
		go_eat(philo);
		if (check_termination(philo->data))
			break ;
		go_sleep(philo);
	}
	return (NULL);
}
