/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:07:32 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/02/04 10:18:21 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "../libft/libft.h"
# include "../libft/get_next_line.h"
# include <stdio.h>

typedef struct s_philo
{
	char  *empty;
}	t_philo;

/* MAIN */
void	ft_error(char *err_msg, t_philo *data);
void	free_philo(t_philo *data);

#endif
