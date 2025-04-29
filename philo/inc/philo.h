/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:07:32 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/04/29 20:29:55 by mpenas-z         ###   ########.fr       */
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
	int				fork_ids[2];
	long			last_eat_milis;
	long			last_change_milis;
	pthread_mutex_t	*philo_mutex;
	pthread_t		*thread;
	struct s_data	*data;
	struct s_philo	*next;
}	t_philo;

typedef struct s_data
{
	int				terminate;
	int				number_of_philos;
	int				threads_ready;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			start_time;
	int				eat_to_finish;
	int				*fork_status;
	pthread_mutex_t	*main_mutex;
	pthread_mutex_t	**fork_mutex;
	t_philo			*philo_list;
}	t_data;

/* UTILITIES */
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_isspace(int c);
int		ft_specialcases(const char *nptr);
int		ft_atoi(const char *nptr);
int		ft_is_str_numeric(char *str);

/* AUXILIARY */
int		launch_threads(t_data *data);
int		join_threads(t_data *data);
void	terminate_simulation(t_data *data);
int		did_philosophers_eat_enough(t_data *data);
long	get_current_milis(t_data *data);

/* FORKS */
int		catch_first_fork(t_philo *philo);
int		catch_second_fork(t_philo *philo);
int		leave_forks(t_philo *philo);
int		has_first_fork(t_philo *philo);
int		has_second_fork(t_philo *philo);

/* INITIALIZER */
int		initialize_philo_data(int argc, char *argv[], t_data **data);
int		initialize_mutexes(t_data *data);
int		initialize_forks(t_data *data);
int		initialize_philosophers(t_data *data);
int		initialize_philo_list(t_data *data);

/* SIMULATION 2 */
int		has_sim_started(t_data *data);
void	custom_sleep(long sleep_micros, t_data *data);
int		go_think(t_philo *philo);
int		go_eat(t_philo *philo);
int		go_sleep(t_philo *philo);
void	*philo_routine(void *arg);

/* SIMULATION 1 */
int		run_simulation(t_data *data);
long	get_last_eat_milis(t_philo *philo);
int		monitor_philosophers(t_data *data);
int		check_termination(t_data *data);
int		kill_philosopher(t_philo *philo);

/* MAIN */
int		ft_error(char *err_msg, t_data *data);
void	free_philo_list(t_philo *list);
void	free_philo_forks(t_data *data);
void	free_philo_data(t_data *data);

#endif
