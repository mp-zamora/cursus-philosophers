/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:07:32 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/02/04 14:14:37 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "../libft/libft.h"
# include "../libft/get_next_line.h"
# include <stdio.h>

typedef struct s_philo
{
	int				number;
	int				status;
	struct s_philo	*next;
	struct s_philo	*prev;
}	t_philo;

typedef struct s_data
{
	t_philo	*philo_list;
	int		number_of_philos;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		eat_to_finish;
}	t_data;

/* INITIALIZER */
void	initialize_data(int argc, char **argv, t_data *data);

/* MAIN */
void	ft_error(char *err_msg, t_data *data);
void	free_philo(t_data *data);

#endif
