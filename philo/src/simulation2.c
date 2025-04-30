/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 12:07:06 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/04/30 16:09:26 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	custom_sleep(long sleep_micros, t_data *data)
{
	long	start;

	start = get_current_ms(data);
	while (get_current_ms(data) - start < sleep_micros / 1000)
		usleep(50);
}

int	go_think(t_philo *philo)
{
	long	current_milis;

	pthread_mutex_lock(philo->philo_mutex);
	philo->status = 1;
	current_milis = get_current_ms(philo->data);
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
	current_milis = get_current_ms(philo->data);
	philo->last_change_milis = current_milis;
	philo->last_eat_milis = current_milis;
	pthread_mutex_unlock(philo->philo_mutex);
	printf("%ld %d is eating.\n", \
		current_milis - philo->data->start_time, philo->number);
	custom_sleep(philo->data->time_to_eat * 1000, philo->data);
	leave_forks(philo);
	pthread_mutex_lock(philo->philo_mutex);
	philo->times_eaten += 1;
	pthread_mutex_unlock(philo->philo_mutex);
	return (0);
}

int	go_sleep(t_philo *philo)
{
	long	current_milis;

	pthread_mutex_lock(philo->philo_mutex);
	philo->status = 0;
	current_milis = get_current_ms(philo->data);
	philo->last_change_milis = current_milis;
	pthread_mutex_unlock(philo->philo_mutex);
	printf("%ld %d is sleeping.\n", \
		current_milis - philo->data->start_time, philo->number);
	custom_sleep(philo->data->time_to_sleep * 1000, philo->data);
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	philo->last_eat_milis = get_current_ms(philo->data);
	philo->last_change_milis = philo->last_eat_milis;
	pthread_mutex_lock(philo->data->main_mutex);
	philo->data->threads_ready++;
	pthread_mutex_unlock(philo->data->main_mutex);
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
