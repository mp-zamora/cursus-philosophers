/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 12:07:06 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/04/29 20:56:31 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void  custom_sleep(long sleep_micros, t_data *data)
{
	long  start;

	start = get_current_milis(data);
	while(get_current_milis(data) - start < sleep_micros / 1000)
	{
		if (check_termination(data))
			break;
		usleep(50);
	}
}

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
		if (!has_second_fork(philo) && has_first_fork(philo))
			catch_second_fork(philo);
		if (check_termination(philo->data) || (has_first_fork(philo)
				&& has_second_fork(philo)))
			break ;
		custom_sleep(100, philo->data);
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
	custom_sleep(philo->data->time_to_eat * 1000, philo->data);
	pthread_mutex_lock(philo->philo_mutex);
	philo->times_eaten += 1;
	pthread_mutex_unlock(philo->philo_mutex);
	leave_forks(philo);
	return (0);
}

int	go_sleep(t_philo *philo)
{
	long	current_milis;

	pthread_mutex_lock(philo->philo_mutex);
	philo->status = 0;
	current_milis = get_current_milis(philo->data);
	philo->last_change_milis = current_milis;
	pthread_mutex_unlock(philo->philo_mutex);
	printf("%ld %d is sleeping.\n", \
		current_milis - philo->data->start_time, philo->number);
	custom_sleep(philo->data->time_to_sleep * 1000, philo->data);
	return (0);
}

int	has_sim_started(t_data *data)
{
	int	has_sim_started;

	has_sim_started = 0;
	pthread_mutex_lock(data->main_mutex);
	if (data->threads_ready == data->number_of_philos)
		has_sim_started = 1;
	pthread_mutex_unlock(data->main_mutex);
	return (has_sim_started);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(philo->philo_mutex);
	philo->last_eat_milis = get_current_milis(philo->data);
	philo->last_change_milis = philo->last_eat_milis;
	pthread_mutex_unlock(philo->philo_mutex);
	pthread_mutex_lock(philo->data->main_mutex);
	philo->data->threads_ready++;
	pthread_mutex_unlock(philo->data->main_mutex);
	while (has_sim_started(philo->data) != 1)
		custom_sleep(100, philo->data);
	pthread_mutex_lock(philo->philo_mutex);
	philo->last_eat_milis = get_current_milis(philo->data);
	philo->last_change_milis = philo->last_eat_milis;
	pthread_mutex_unlock(philo->philo_mutex);
	while (1)
	{
		go_think(philo);
		if (check_termination(philo->data))
			break ;
		go_eat(philo);
		if (check_termination(philo->data))
			break ;
		go_sleep(philo);
		if (check_termination(philo->data))
			break ;
	}
	return (NULL);
}
