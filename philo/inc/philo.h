/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:07:32 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/02/06 14:38:27 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/* DEFINE STATUS CODES */
# define SUCCESS 0
# define FAILURE 1
# define FALSE 0
# define TRUE 1
# define EXIT_FAILURE 1
# define EXIT_SUCCESS 0

/* INCLUDE USEFUL AND ALLOWED FUNCTIONS */
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

/* DEFINE DATA STRUCTURES */
typedef struct s_philo
{
	int				number;
	int				status;
	int				times_eaten;
	int				fork_number;
	long			last_eat_milis;
	long			last_change_milis;
	pthread_t		*thread;
	struct s_data	*data;
	struct s_philo	*next;
}	t_philo;

typedef struct s_data
{
	int				terminate;
	int				number_of_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				eat_to_finish;
	pthread_mutex_t	**forks;
	t_philo			*philo_list;
}	t_data;

/* UTILITIES */
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_isspace(int c);
int		ft_specialcases(const char *nptr);
int		ft_atoi(const char *nptr);
long	get_current_milis(t_data *data);

/* AUXILIARY */
void	initialize_philo_data(int argc, char **argv, t_data **data);
void	initialize_forks(t_data *data);
void	launch_philo_threads(t_data *data);
void	initialize_philo_list(t_data *data);
int		did_philosophers_eat_enough(t_data *data);

/* SIMULATION 2 */
void	kill_philosopher(t_philo *philo);
void	catch_forks(t_philo *philo);
void	leave_forks(t_philo *philo);

/* SIMULATION 1 */
void	monitor_philosophers(t_data *data);
void	go_think(t_philo *philo);
void	go_eat(t_philo *philo);
void	*philo_routine(void *arg);

/* MAIN */
void	ft_error(char *err_msg, t_data *data);
void	free_philo_list(t_philo *list);
void	free_philo_data(t_data *data);

#endif
