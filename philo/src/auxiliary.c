/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auxiliary.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 12:07:59 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/02/06 12:40:35 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	initialize_philo_data(int argc, char *argv[], t_data **data)
{
	int	i;

	(*data) = (t_data *)malloc(sizeof(t_data));
	if (!(*data))
		ft_error("Failure initializing the data structure.", NULL);
	(*data)->number_of_philos = ft_atoi(argv[1]);
	(*data)->time_to_die = ft_atoi(argv[2]);
	(*data)->time_to_eat = ft_atoi(argv[3]);
	(*data)->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6 && argv[5])
		(*data)->eat_to_finish = ft_atoi(argv[5]);
	else
		(*data)->eat_to_finish = -1;
	(*data)->forks = (pthread_mutex_t **)malloc(sizeof(pthread_mutex_t *)
			* (*data)->number_of_philos);
	if (!(*data)->forks)
		ft_error("Failure initializing the data structure.", (*data));
	i = -1;
	while (++i < (*data)->number_of_philos)
	{
		(*data)->forks[i] = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		if (!(*data)->forks[i])
			ft_error("Failure in fork malloc.\n", (*data));
		if (pthread_mutex_init((*data)->forks[i], NULL) != 0)
			ft_error("Failure initializing mutex.\n", (*data));
	}
	(*data)->philo_list = NULL;
}

void	launch_philo_threads(t_data *data)
{
	t_philo	*iter;
	int		num;
	long	current_milis;

	initialize_philo_list(data);
	iter = data->philo_list;
	num = 0;
	while (iter && ++num <= data->number_of_philos)
	{
		iter->number = num;
		iter->status = 0;
		iter->times_eaten = 0;
		current_milis = get_current_milis(data);
		iter->last_eat_milis = current_milis;
		iter->last_change_milis = current_milis;
		iter->data = data;
		if (pthread_create(iter->thread, NULL,
				philo_routine, (void *)iter) != 0)
			ft_error("Failure creating a thread.", data);
		iter = iter->next;
	}
}

void	initialize_philo_list(t_data *data)
{
	t_philo	*iter;
	int		num;

	iter = (t_philo *)malloc(sizeof(t_philo));
	if (!iter)
		ft_error("Failure on t_philo malloc.\n", data);
	data->philo_list = iter;
	num = 0;
	while (++num <= data->number_of_philos)
	{
		iter->next = (t_philo *)malloc(sizeof(t_philo));
		if (!iter)
			ft_error("Failure on t_philo malloc.\n", data);
		iter->thread = (pthread_t *)malloc(sizeof(pthread_t));
		if (!iter->thread)
			ft_error("Failure on thread malloc.\n", data);
		iter = iter->next;
	}
	iter->next = NULL;
}

long	get_current_milis(t_data *data)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		ft_error("Failure fetching current time.", data);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	did_philosophers_eat_enough(t_data *data)
{
	t_philo	*iter;

	if (data->eat_to_finish == -1)
		return (FALSE);
	iter = data->philo_list;
	while (iter)
	{
		if (iter->times_eaten == data->eat_to_finish)
			iter = iter->next;
		else
			return (FALSE);
	}
	return (TRUE);
}
