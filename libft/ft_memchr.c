/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 17:39:39 by lyoussef          #+#    #+#             */
/*   Updated: 2024/06/15 17:52:49 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char		*p;
	unsigned char		ch;

	p = (unsigned char *)s;
	ch = (unsigned char)c;
	while (n > 0)
	{
		if (*p == ch)
		{
			return (p);
		}
		p++;
		n--;
	}
	return (NULL);
}
