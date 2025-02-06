/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 10:17:12 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/02/06 10:46:36 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

/* UTILITIES */
int	ft_isspace(int c)
{
	if (c == ' ' || c == '\f' || c == '\n'
		|| c == '\r' || c == '\t' || c == '\v')
		return (1);
	return (0);
}

int	ft_specialcases(const char *nptr)
{
	if (ft_strncmp("2147483647", nptr, 10) == 0)
		return (2147483647);
	else if (ft_strncmp("-2147483648", nptr, 11) == 0)
		return (-2147483648);
	return (0);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	result;
	int	minus;

	i = 0;
	result = 0;
	minus = 1;
	while (ft_isspace(nptr[i]))
		i++;
	while (nptr[i] && ((nptr[i] >= 48 && nptr[i] <= 57)
			|| (nptr[i] == '+' && (i == 0 || ft_isspace(nptr[i - 1])))
			|| (nptr[i] == '-' && (i == 0 || ft_isspace(nptr[i - 1])))))
	{
		if (nptr[i] == '-')
			minus = -1;
		if (nptr[i] >= 48 && nptr[i] <= 57)
			result = result * 10 + (nptr[i] - '0');
		i++;
	}
	if (ft_specialcases(nptr) != 0)
		return (ft_specialcases(nptr));
	return (result * minus);
}

/* AUXILIARY */
// Forks probably should be mutex's, thats how we would know if they are being used.
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
	data->forks = (pthread_mutex_t **)malloc(sizeof(pthread_mutex_t *)
			* data->number_of_philos);
	if (!data->forks)
		ft_error("Failure initializing the data structure.", data);
	i = -1;
	while (++i < data->number_of_philos)
		pthread_mutex_init(data->forks[i], NULL);
	data->philo_list = NULL;
}

long	get_current_milis(t_data *data)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		ft_error("Failure fetching current time.", data);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	kill_philosopher(t_philo *philo)
{
	(void)philo;
	(void)data;
	return (FAILURE);
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
				return (kill_philosopher(iter));
			iter = iter->next;
		}
		if (did_philosophers_eat_enough(data))
			return (SUCCESS);
	}
	return (FAILURE);
}

void	catch_forks(t_philo *philo)
{
	long	current_milis;

	if (philo->number == philo->data->number_of_philos)
	{
		current_milis = get_current_milis(philo->data);
		printf("%l %d has taken a fork.\n", current_milis, philo->number);
		pthread_mutex_lock(philo->data->forks[0]);
	}
	pthrad_mutex_lock(philo->data->forks[philo->number - 1]);
	current_milis = get_current_milis(philo->data);
	printf("%l %d has taken a fork.\n", current_milis, philo->number);
	if (philo->number < philo->data->number_of_philos)
	{
		pthread_mutex_lock(philo->data->forks[philo->number]);
		current_milis = get_current_milis(philo->data);
		printf("%l %d has taken a fork.\n", current_milis, philo->number);
	}
}

void	leave_forks(t_philo *philo)
{
	if (philo->number == philo->data->number_of_philos)
		pthread_mutex_unlock(philo->data->forks[0]);
	pthrad_mutex_unlock(philo->data->forks[philo->number - 1]);
	if (philo->number < philo->data->number_of_philos)
		pthread_mutex_unlock(philo->data->forks[philo->number]);
}

void	go_think(t_philo *philo)
{
	long	current_milis;

	philo->status = 1;
	current_milis = get_current_milis(philo->data);
	philo->last_change_milis = current_milis;
	printf("%l %d is thinking.\n", current_milis, philo->number);
	catch_forks(philo);
}

void	go_eat(t_philo *philo)
{
	long	current_milis;

	philo->status = 2;
	current_milis = get_current_milis(philo->data);
	philo->last_change_milis = current_milis;
	printf("%l %d is eating.\n", current_milis, philo->number);
	usleep(philo->data->time_to_eat * 100);
	current_milis = get_current_milis(philo->data);
	philo->last_eat_milis = current_milis;
	philo->times_eaten++;
	leave_forks(philo);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	long	current_milis;

	philo = (t_philo *)arg;
	while (1)
	{
		philo->status = 0;
		current_milis = get_current_milis(philo->data);
		philo->last_change_milis = current_milis;
		printf("%l %d is sleeping.\n", current_milis, philo->number);
		usleep(philo->data->time_to_sleep * 100);
		go_think(philo);
		go_eat(philo);
	}
	return (NULL);
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
	printf("Error: %s\n", err_msg);
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
		if (list->thread)
		{
			pthread_join(list->thread);
			free (list->thread);
		}
		aux = list->next;
		free (list);
		list = aux;
	}
}

void	free_philo_data(t_data *data)
{
	int	i;

	if (!data)
		return ;
	if (data->philo_list)
		free_philo_list(data->philo_list);
	if (data->forks)
	{
		i = -1;
		while (data->forks[++i])
		{
			pthread_mutex_destroy(data->forks[i]);
			free (data->forks[i]);
		}
		free (data->forks);
	}
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
