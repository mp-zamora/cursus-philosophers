/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 12:08:29 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/04/10 13:30:14 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i] && i < n - 1)
		i++;
	if (i != n)
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	return (0);
}

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

int	ft_is_str_numeric(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] < '0' || str[i] > '9')
			return (FALSE);
	return (TRUE);
}
