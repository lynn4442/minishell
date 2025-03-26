/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:44:59 by lyoussef          #+#    #+#             */
/*   Updated: 2024/09/02 18:21:12 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char	*last;
	const char	*start;
	char		occurence;

	last = NULL;
	start = s;
	occurence = (char)c;
	while (*start)
	{
		if (*start == occurence)
		{
			last = start;
		}
		start++;
	}
	if (occurence == '\0')
		return ((char *)start);
	return ((char *)last);
}
