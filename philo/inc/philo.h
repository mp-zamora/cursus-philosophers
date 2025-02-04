/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:07:32 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/02/04 10:32:09 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "../libft/libft.h"
# include "../libft/get_next_line.h"
# include <stdio.h>

typedef struct s_philo
{
	int	number_of_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	eat_to_finish;
}	t_philo;

/* INITIALIZER */
void	initialize_philo(int argc, char **argv, t_philo *data);

/* MAIN */
void	ft_error(char *err_msg, t_philo *data);
void	free_philo(t_philo *data);

#endif
