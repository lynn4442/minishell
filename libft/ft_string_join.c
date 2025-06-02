/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string_join.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/01/19 16:00:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2, t_gc *x)
{
	size_t	i;
	size_t	lens1;
	size_t	lens2;
	char	*result;

	i = 0;
	lens1 = ft_strlen(s1);
	lens2 = ft_strlen(s2);
	result = ft_malloc(x, sizeof(char) * (lens1 + lens2 + 1));
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

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	slen;
	size_t	dlen;

	if (size <= ft_strlen(dst))
		return (size + ft_strlen(src));
	dlen = ft_strlen(dst);
	slen = 0;
	while (src[slen] != '\0' && dlen + 1 < size)
	{
		dst[dlen] = src[slen];
		dlen++;
		slen++;
	}
	dst[dlen] = '\0';
	return (ft_strlen(dst) + ft_strlen(&src[slen]));
}
