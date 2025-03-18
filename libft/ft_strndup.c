/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 10:27:57 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/18 11:36:17 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(char *s, size_t len)
{
	char	*dup_str;

	dup_str = malloc(len + 1);
	if (!dup_str)
		return (NULL);
	ft_strncpy(dup_str, s, len);
	dup_str[len] = '\0';
	return (dup_str);
}
