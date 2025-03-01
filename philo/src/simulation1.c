/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 12:05:12 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/03/01 17:02:40 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	monitor_philosophers(t_data *data)
{
	t_philo		*iter;
	long		current_milis;

	while (data->terminate != 1)
	{
		usleep(5000);
		iter = data->philo_list;
		while (iter)
		{
			current_milis = get_current_milis(data);
			if (current_milis - iter->last_eat_milis > data->time_to_die)
				return (kill_philosopher(iter));
			iter = iter->next;
		}
		if (did_philosophers_eat_enough(data) == TRUE)
		{
			pthread_mutex_lock(data->main_mutex);
			data->terminate = 1;
			pthread_mutex_unlock(data->main_mutex);
			free_philo_data(data);
			return (0);
		}
	}
	return (0);
}

int	go_think(t_philo *philo)
{
	long	current_milis;

	if (philo->data->terminate == 1 || philo->status == -1)
		return (1);
	pthread_mutex_lock(philo->data->main_mutex);
	philo->status = 1;
	current_milis = get_current_milis(philo->data);
	philo->last_change_milis = current_milis;
	pthread_mutex_unlock(philo->data->main_mutex);
	printf("%ld %d is thinking.\n", current_milis - philo->data->start_time, philo->number);
	while ((philo->fork_ids[1] == -1 || philo->fork_ids[0] == -1)
		&& philo->data->terminate != 1 && philo->status != -1)
	{
		if (catch_first_fork(philo) != 0)
			return (1);
		if (catch_second_fork(philo) != 0)
			return (1);
	}
	return (0);
}

int	go_eat(t_philo *philo)
{
	long	current_milis;

	if (philo->data->terminate == 1 || philo->status == -1)
		return (1);
	pthread_mutex_lock(philo->data->main_mutex);
	philo->status = 2;
	current_milis = get_current_milis(philo->data);
	philo->last_change_milis = current_milis;
	philo->last_eat_milis = current_milis;
	pthread_mutex_unlock(philo->data->main_mutex);
	printf("%ld %d is eating.\n", current_milis - philo->data->start_time, philo->number);
	usleep(philo->data->time_to_eat * 1000);
	pthread_mutex_lock(philo->data->main_mutex);
	philo->times_eaten++;
	pthread_mutex_unlock(philo->data->main_mutex);
	leave_forks(philo);
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	long	current_milis;

	philo = (t_philo *)arg;
	while (philo->data->terminate != 1)
	{
		pthread_mutex_lock(philo->data->main_mutex);
		philo->status = 0;
		current_milis = get_current_milis(philo->data);
		philo->last_change_milis = current_milis;
		pthread_mutex_unlock(philo->data->main_mutex);
		printf("%ld %d is sleeping.\n", current_milis - philo->data->start_time, philo->number);
		usleep(philo->data->time_to_sleep * 1000);
		if (go_think(philo) != 0)
			return (NULL);
		if (go_eat(philo) != 0)
			return (NULL);
	}
	return (NULL);
}
