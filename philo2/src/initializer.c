/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 10:38:02 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/04/09 12:09:11 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	initialize_philo_data(int argc, char *argv[], t_data **data)
{
	(*data) = (t_data *)malloc(sizeof(t_data));
	if (!(*data))
		return (ft_error("Failure initializing the data structure.", NULL));
	(*data)->terminate = 0;
	(*data)->number_of_philos = ft_atoi(argv[1]);
	(*data)->time_to_die = ft_atoi(argv[2]);
	(*data)->time_to_eat = ft_atoi(argv[3]);
	(*data)->time_to_sleep = ft_atoi(argv[4]);
	(*data)->start_time = get_current_milis(*data);
	if (argc == 6 && argv[5])
		(*data)->eat_to_finish = ft_atoi(argv[5]);
	else
		(*data)->eat_to_finish = -1;
	if (initialize_philosophers(*data) != 0)
		return (1);
	if (initialize_mutexes(*data) != 0)
		return (ft_error("Failure initializing mutex.", *data));
	return (0);
}

int	initialize_mutexes(t_data *data)
{
	data->main_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (!data->main_mutex)
		return (ft_error("Failure creating main mutex.", data));
	if (pthread_mutex_init(data->main_mutex, NULL) != 0)
		return (ft_error("Failure initializing mutex.\n", data));
	if (initialize_forks(data) != 0)
		return (1);
	return (0);
}

int	initialize_forks(t_data *data)
{
	int	i;

	data->fork_mutex = (pthread_mutex_t **)malloc(sizeof(pthread_mutex_t *)
			* data->number_of_philos);
	data->fork_status = (int *)malloc(sizeof(int) * data->number_of_philos);
	if (!data->fork_status || !data->fork_mutex)
		return (ft_error("Failure initializing the data structure.", data));
	i = -1;
	while (++i < data->number_of_philos)
	{
		data->fork_mutex[i] = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		if (!data->fork_mutex[i])
			return (ft_error("Failure in fork malloc.\n", data));
		if (pthread_mutex_init(data->fork_mutex[i], NULL) != 0)
			return (ft_error("Failure initializing mutex.\n", data));
		data->fork_status[i] = 0;
	}
	return (0);
}

int	initialize_philosophers(t_data *data)
{
	t_philo	*iter;
	int		num;

	if (initialize_philo_list(data) != 0)
		return (1);
	iter = data->philo_list;
	num = 0;
	while (iter && ++num <= data->number_of_philos)
	{
		iter->number = num;
		iter->status = 0;
		iter->times_eaten = 0;
		iter->fork_ids[0] = -1;
		iter->fork_ids[1] = -1;
		iter->data = data;
		iter->philo_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		if (!iter->philo_mutex)
			return (ft_error("Failure in philo malloc.\n", data));
		if (pthread_mutex_init(iter->philo_mutex, NULL) != 0)
			return (ft_error("Failure initializing mutex.\n", data));
		iter = iter->next;
	}
	return (0);
}

int	initialize_philo_list(t_data *data)
{
	t_philo	*iter;
	int		num;

	iter = (t_philo *)malloc(sizeof(t_philo));
	if (!iter)
		return (ft_error("Failure on t_philo malloc.\n", data));
	data->philo_list = iter;
	num = 0;
	while (++num <= data->number_of_philos)
	{
		iter->thread = (pthread_t *)malloc(sizeof(pthread_t));
		if (!iter->thread)
			return (ft_error("Failure on thread malloc.\n", data));
		if (num < data->number_of_philos)
		{
			iter->next = (t_philo *)malloc(sizeof(t_philo));
			if (!iter)
				return (ft_error("Failure on t_philo malloc.\n", data));
			iter = iter->next;
		}
	}
	iter->next = NULL;
	return (0);
}
