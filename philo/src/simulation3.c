/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 14:23:46 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/05/03 14:28:12 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	init_philo(t_philo *philo)
{
	philo->last_eat_milis = get_current_ms(philo->data);
	philo->last_change_milis = philo->last_eat_milis;
	pthread_mutex_lock(philo->data->main_mutex);
	philo->data->threads_ready++;
	pthread_mutex_unlock(philo->data->main_mutex);
	while (!has_sim_started(philo->data))
		custom_sleep(100, philo->data);
	philo->last_eat_milis = get_current_ms(philo->data);
	philo->last_change_milis = philo->last_eat_milis;
}
