/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 17:53:47 by lyoussef          #+#    #+#             */
/*   Updated: 2024/06/16 20:47:36 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_in_set(char c, char const *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

static int	ft_start(char const *s1, char const *set)
{
	int	i;

	i = 0;
	while (s1[i] && is_in_set(s1[i], set))
		i++;
	return (i);
}

static int	ft_end(char const *s1, char const *set)
{
	int	i;

	i = 0;
	while (s1[i])
		i++;
	i--;
	while (i >= 0 && is_in_set(s1[i], set))
		i--;
	return (i + 1);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	int		len;
	char	*result;

	if (!s1)
		return (NULL);
	start = ft_start(s1, set);
	end = ft_end(s1, set);
	len = end - start;
	if (len < 0)
		len = 0;
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	result[len] = '\0';
	while (len--)
		result[len] = s1[start + len];
	return (result);
}
