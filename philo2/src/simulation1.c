/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 12:05:12 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/04/09 00:37:03 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

// SUBDIVIDE IN CHECK DEATH; CHECK EAT ENOUGH AND CHECK TERMINATION VAR FUCNTIONS
int	monitor_philosophers(t_data *data)
{
	t_philo		*iter;
	long		current_milis;
	long		last_eat_milis;

	while (1)
	{
		iter = data->philo_list;
		while (iter)
		{
			pthread_mutex_lock(iter->philo_mutex);
			last_eat_milis = iter->last_eat_milis;
			pthread_mutex_unlock(iter->philo_mutex);
			current_milis = get_current_milis(data);
			if (current_milis - last_eat_milis > data->time_to_die)
			{
				kill_philosopher(iter);
				break ;
			}
			iter = iter->next;
		}
		if (did_philosophers_eat_enough(data) == TRUE)
			terminate_simulation(data);
		if (check_termination(data))
			break ;
		else
			usleep(5000);
	}
	return (0);
}

int	check_termination(t_data *data)
{
	int	terminate;

	pthread_mutex_lock(data->main_mutex);
	terminate = data->terminate;
	pthread_mutex_unlock(data->main_mutex);
	return (terminate);
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

// DIVIDE INTO PROPER SLEEP; THINK; EAT. BEGIN WITH THINK, THEN EAT; FINALLY SLEEP.
void	*philo_routine(void *arg)
{
	t_philo	*philo;
	long	current_milis;

	philo = (t_philo *)arg;
	while (1)
	{
		if (check_termination(philo->data))
			break ;
		pthread_mutex_lock(philo->philo_mutex);
		philo->status = 0;
		philo->last_change_milis = current_milis;
		pthread_mutex_unlock(philo->philo_mutex);
		current_milis = get_current_milis(philo->data);
		printf("%ld %d is sleeping.\n", current_milis - philo->data->start_time, philo->number);
		usleep(philo->data->time_to_sleep * 1000);
		if (go_think(philo) != 0)
			return (NULL);
		if (go_eat(philo) != 0)
			return (NULL);
	}
	return (NULL);
}
