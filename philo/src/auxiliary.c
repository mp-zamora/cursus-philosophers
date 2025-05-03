/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auxiliary.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 12:07:59 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/05/03 14:17:16 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	launch_threads(t_data *data)
{
	t_philo	*iter;

	iter = data->philo_list;
	while (iter)
	{
		if (pthread_create(iter->thread, NULL, philo_routine, iter) != 0)
			return (ft_error("Failure creating a thread.", data));
		iter = iter->next;
	}
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

void	terminate_simulation(t_data *data)
{
	if (!data || !data->main_mutex)
		return ;
	pthread_mutex_lock(data->main_mutex);
	data->terminate = 1;
	pthread_mutex_unlock(data->main_mutex);
}

int	did_philosophers_eat_enough(t_data *data)
{
	t_philo	*iter;
	int		philo_count;

	if (data->eat_to_finish == -1)
		return (FALSE);
	philo_count = 0;
	iter = data->philo_list;
	while (iter)
	{
		pthread_mutex_lock(iter->philo_mutex);
		if (iter->times_eaten >= data->eat_to_finish)
			philo_count++;
		pthread_mutex_unlock(iter->philo_mutex);
		iter = iter->next;
	}
	if (philo_count == data->number_of_philos)
		return (TRUE);
	return (FALSE);
}

long	get_current_ms(t_data *data)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		ft_error("Failure fetching current time.", data);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
