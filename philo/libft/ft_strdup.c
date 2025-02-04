/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpenas-z <mpenas-z@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 11:29:12 by mpenas-z          #+#    #+#             */
/*   Updated: 2025/01/23 17:38:18 by mpenas-z         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	int		i;
	int		len;
	char	*str;

	len = ft_strlen(s);
	str = (char *)malloc(sizeof(*str) * (len + 1));
	if (!str)
		return (NULL);
	i = -1;
	while (++i < len)
		str[i] = s[i];
	str[i] = '\0';
	return (str);
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*res;

	if (!s || n <= 0)
		return (ft_strdup(""));
	if (n >= ft_strlen(s))
		n = ft_strlen(s);
	res = malloc((n + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);
	ft_strlcpy(res, s, n + 1);
	return (res);
}
