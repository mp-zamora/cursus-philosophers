/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 10:17:12 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/02/06 09:34:11 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

/* AUXILIARY */

void	initialize_philo_data(int argc, char *argv[], t_data *data)
{
	int	i;

	data = (t_data *)malloc(sizeof(data));
	if (!data)
		ft_error("Failure initializing the data structure.", data);
	data->number_of_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6 && argv[5])
		data->eat_to_finish = ft_atoi(argv[5]);
	else
		data->eat_to_finish = -1;
	data->forks = (int *)malloc(sizeof(int) * data->number_of_philos);
	if (!data->forks)
		ft_error("Failure initializing the data structure.", data);
	i = -1;
	while (++i < data->number_of_philos)
		data->forks[i] = 0;
	data->philo_list = NULL;
	/* SHould probably initialize MUTEX here */
	data->mutex = NULL;
}

long	get_current_milis(t_data *data)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		ft_error("Failure fetching current time.", data);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));

}

int	kill_philosopher(t_philo *philo, t_data *data)
{
	(void)philo;
	(void)data;
	return (FAILURE);
}

int	did_philosophers_eat_enough(t_data *data)
{
	t_philo	*iter;

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

/* SIMULATION */

int	monitor_philosophers(t_data *data)
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
				return (kill_philosopher(iter, data));
			iter = iter->next;
		}
		if (did_philosophers_eat_enough(data))
			return (SUCCESS);
	}
	return (FAILURE);
}

void	*philo_routine(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	while (1)
	{
		go_sleep(philo);
		go_think(philo);
		go_eat(philo);
	}
}

int	launch_philo_threads(t_data *data)
{
	t_philo	*iter;
	int		num;
	long	current_milis;

	iter = data->philo_list;
	num = 0;
	while (iter && num <= data->number_of_philos)
	{
		iter->number = ++num;
		iter->status = 0;
		iter->times_eaten = 0;
		current_milis = get_current_milis(data);
		iter->last_eat_milis = current_milis;
		iter->last_change_milis = current_milis;
		iter->data = data;
		pthread_create(iter->thread, NULL, philo_routine, (void *)iter);
		iter = iter->next;
	}
	return (1);
}

/* MAIN */

void	ft_error(char *err_msg, t_data *data)
{
	ft_putstr_fd("Error: ", 2);
	ft_putendl_fd(err_msg, 2);
	free_philo_data(data);
	exit (EXIT_FAILURE);
}

void	free_philo_list(t_philo *list)
{
	t_philo	*aux;

	if (!list)
		return ;
	while (list)
	{
		/* Should I free it or close it? */
		if (list->thread)
			free (list->thread);
		aux = list->next;
		free (list);
		list = aux;
	}
}

void	free_philo_data(t_data *data)
{
	if (!data)
		return ;
	/* Should I free it or close it? */
	if (data->mutex)
		free (data->mutex);
	if (data->forks)
		free (data->forks);
	if (data->philo_list)
		free_philo_list(data->philo_list);
	free (data);
}

int	main(int argc, char *argv[])
{
	t_data	*data;

	if (argc != 5 && argc != 6)
		ft_error("Not enough args!", NULL);
	initialize_philo_data(argc, argv, &data);
	launch_philo_threads(data);
	monitor_philosophers(data);
	free_philo_data(data);
	return (EXIT_SUCCESS);
}
