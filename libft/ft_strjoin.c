/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_result.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 17:36:25 by lyoussef          #+#    #+#             */
/*   Updated: 2024/06/19 13:23:25 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2,t_gc *x)
{
	size_t	i;
	size_t	lens1;
	size_t	lens2;
	char	*result;

	i = 0;
	lens1 = ft_strlen(s1);
	lens2 = ft_strlen(s2);
	result = ft_malloc( x, sizeof(char) * (lens1 + lens2 + 1));
	if (!s1 || !s2)
		return (NULL);
	if (!result)
		return (NULL);
	result[lens1 + lens2] = '\0';
	while (*s1)
		result[i++] = *s1++;
	while (*s2)
		result[i++] = *s2++;
	return (result);
}
