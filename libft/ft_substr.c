/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 16:40:06 by lyoussef          #+#    #+#             */
/*   Updated: 2024/06/19 13:28:54 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	slen;
	char	*substr;

	slen = ft_strlen(s);
	if (start >= slen)
	{
		return (ft_calloc(1, sizeof(char)));
	}
	if (len > slen - start)
	{
		len = slen - start;
	}
	substr = malloc(len + 1);
	if (!substr)
	{
		return (NULL);
	}
	ft_memcpy(substr, s + start, len);
	substr[len] = '\0';
	return (substr);
}
