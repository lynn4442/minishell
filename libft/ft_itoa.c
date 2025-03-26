/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 17:55:17 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/22 19:15:21 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_size(int n)
{
	size_t			size;
	long long int	tmp;

	size = 0;
	tmp = n;
	if (tmp < 0)
	{
		size++;
		tmp = -tmp;
	}
	while (tmp > 0)
	{
		size++;
		tmp /= 10;
	}
	if (n == 0)
		size = 1;
	return (size);
}

static void	ft_ans(char *str, int n, size_t size)
{
	long long int	tmp;
	size_t			i;

	tmp = n;
	i = size;
	if (n < 0)
	{
		str[0] = '-';
		tmp = -tmp;
	}
	str[i--] = '\0';
	while (tmp > 0)
	{
		str[i--] = tmp % 10 + '0';
		tmp /= 10;
	}
	if (n == 0)
		str[0] = '0';
}

char	*ft_itoa(int n, t_gc *gc)
{
	char	*str;
	size_t	size;

	size = ft_size(n);
	str = (char *)ft_malloc( gc, size + 1);
	if (!str)
		return (NULL);
	ft_ans(str, n, size);
	return (str);
}
