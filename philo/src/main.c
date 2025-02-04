/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 10:17:12 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/02/04 10:27:16 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	main(int agrc, char *argv[])
{
	t_philo	*data;

	if (agrc != 5 && argc != 6)
		return (ft_putstr_fd("Not enough arguments!\n", 2), 1);
	initialize_philo(argc, argv, &data);
	printf("¡Hola mundo!\n");
}
