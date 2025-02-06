/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 12:05:12 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/02/06 20:44:31 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	monitor_philosophers(t_data *data)
{
	t_philo		*iter;
	long		current_milis;

	while (1)
	{
		usleep(5000);
		iter = data->philo_list;
		while (iter)
		{
			current_milis = get_current_milis(data);
			if (current_milis - iter->last_eat_milis > data->time_to_die)
				kill_philosopher(iter);
			iter = iter->next;
		}
		if (did_philosophers_eat_enough(data) == TRUE)
		{
			data->terminate = 1;
			free_philo_data(data);
			exit(EXIT_SUCCESS);
		}
	}
}

void	go_think(t_philo *philo)
{
	long	current_milis;

	if (philo->data->terminate == 1)
		return ;
	philo->status = 1;
	current_milis = get_current_milis(philo->data);
	philo->last_change_milis = current_milis;
	printf("%ld %d is thinking.\n", current_milis, philo->number);
	while ((philo->fork_ids[1] == -1 || philo->fork_ids[0] == -1)
		&& philo->data->terminate != 1)
	{
		catch_first_fork(philo);
		catch_second_fork(philo);
	}
}

void	go_eat(t_philo *philo)
{
	long	current_milis;

	if (philo->data->terminate == 1)
		return ;
	philo->status = 2;
	current_milis = get_current_milis(philo->data);
	philo->last_change_milis = current_milis;
	philo->last_eat_milis = current_milis;
	printf("%ld %d is eating.\n", current_milis, philo->number);
	usleep(philo->data->time_to_eat * 100);
	current_milis = get_current_milis(philo->data);
	philo->times_eaten++;
	leave_forks(philo);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	long	current_milis;

	philo = (t_philo *)arg;
	while (philo->data->terminate != 1)
	{
		philo->status = 0;
		current_milis = get_current_milis(philo->data);
		philo->last_change_milis = current_milis;
		printf("%ld %d is sleeping.\n", current_milis, philo->number);
		usleep(philo->data->time_to_sleep * 100);
		go_think(philo);
		go_eat(philo);
	}
	return (NULL);
}
