/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 12:07:06 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/05/03 16:38:13 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	custom_sleep(long sleep_micros, t_data *data)
{
	long	start;

	start = get_current_ms(data);
	while (get_current_ms(data) - start < sleep_micros / 1000)
	{
		if (check_termination(data))
			break ;
		usleep(50);
	}
}

int	go_think(t_philo *philo)
{
	long	current_milis;

	pthread_mutex_lock(philo->data->write_mutex);
	current_milis = get_current_ms(philo->data);
	printf("%ld %d is thinking.\n", \
		current_milis - philo->data->start_time, philo->number);
	pthread_mutex_unlock(philo->data->write_mutex);
	pthread_mutex_lock(philo->philo_mutex);
	philo->status = 1;
	philo->last_change_milis = current_milis;
	pthread_mutex_unlock(philo->philo_mutex);
	if (philo->number % 2 == 0 && philo->times_eaten == 0)
		custom_sleep(philo->data->time_to_eat * 500, philo->data);
	catch_first_fork(philo);
	if (philo->data->number_of_philos != 1)
		catch_second_fork(philo);
	return (0);
}

int	go_eat(t_philo *philo)
{
	long	current_milis;

	pthread_mutex_lock(philo->data->write_mutex);
	current_milis = get_current_ms(philo->data);
	printf("%ld %d is eating.\n", \
		current_milis - philo->data->start_time, philo->number);
	pthread_mutex_unlock(philo->data->write_mutex);
	pthread_mutex_lock(philo->philo_mutex);
	philo->status = 2;
	philo->last_change_milis = current_milis;
	philo->last_eat_milis = current_milis;
	philo->times_eaten += 1;
	pthread_mutex_unlock(philo->philo_mutex);
	custom_sleep(philo->data->time_to_eat * 1000, philo->data);
	leave_forks(philo);
	return (0);
}

int	go_sleep(t_philo *philo)
{
	long	current_milis;

	pthread_mutex_lock(philo->data->write_mutex);
	current_milis = get_current_ms(philo->data);
	printf("%ld %d is sleeping.\n", \
		current_milis - philo->data->start_time, philo->number);
	pthread_mutex_unlock(philo->data->write_mutex);
	pthread_mutex_lock(philo->philo_mutex);
	philo->status = 0;
	philo->last_change_milis = current_milis;
	pthread_mutex_unlock(philo->philo_mutex);
	custom_sleep(philo->data->time_to_sleep * 1000, philo->data);
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	init_philo(philo);
	while (1)
	{
		go_think(philo);
		if (philo->data->number_of_philos == 1)
			custom_sleep(philo->data->time_to_die * 1000, philo->data);
		if (check_termination(philo->data))
		{
			leave_forks(philo);
			break ;
		}
		go_eat(philo);
		if (check_termination(philo->data))
			break ;
		go_sleep(philo);
		if (check_termination(philo->data))
			break ;
	}
	return (NULL);
}
