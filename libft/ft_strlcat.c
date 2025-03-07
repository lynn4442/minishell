/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 11:28:44 by lyoussef          #+#    #+#             */
/*   Updated: 2024/06/21 09:29:04 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
