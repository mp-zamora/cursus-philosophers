/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:07:32 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/02/06 09:33:42 by mpenas-z         ###   ########.fr       */
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
# include "../libft/libft.h"
# include "../libft/get_next_line.h"
# include <stdio.h>
# include <sys/time.h>

/* DEFINE DATA STRUCTURES */
typedef struct s_philo
{
	int				number;
	int				status;
	int				times_eaten;
	long			last_eat_milis;
	long			last_change_milis;
	pthread_t		*thread;
	t_data			*data;
	struct s_philo	*next;
}	t_philo;

typedef struct s_data
{
	int				number_of_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				eat_to_finish;
	int				*forks;
	t_philo			*philo_list;
	pthread_mutex_t	*mutex;
}	t_data;

/* INITIALIZER */
void	initialize_philo_data(int argc, char **argv, t_data *data);

/* MAIN */
void	ft_error(char *err_msg, t_data *data);
int		monitor_philosophers(t_data *data);
int		launch_philo_threads(t_data *data);
void	free_philo_data(t_data *data);

#endif
