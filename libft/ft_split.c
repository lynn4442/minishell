/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 17:54:29 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/21 00:42:20 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_wordcounter(const char *str, char c)
{
	int	i;
	int	trigger;

	i = 0;
	trigger = 0;
	while (*str)
	{
		if (*str != c && trigger == 0)
		{
			trigger = 1;
			i++;
		}
		else if (*str == c)
			trigger = 0;
		str++;
	}
	return (i);
}

static char	*ft_word(t_gc *free,const char *str, int start, int finish)
{
	char	*word;
	int		i;

	i = 0;
	word = ft_malloc(free,(finish - start + 1) * sizeof(char));
	while (start < finish)
		word[i++] = str[start++];
	word[i] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c,t_gc *free)
{
	size_t	i;
	size_t	j;
	int		index;
	char	**strings;

	i = 0;
	j = 0;
	strings = ft_malloc(free,(ft_wordcounter(s, c) + 1) * sizeof(char *));
	if (!s || !strings)
		return (0);
	index = -1;
	while (i <= ft_strlen(s))
	{
		if (s[i] != c && index < 0)
			index = i;
		else if ((s[i] == c || i == ft_strlen(s)) && index >= 0)
		{
			strings[j++] = ft_word(free,s, index, i);
			index = -1;
		}
		i++;
	}
	strings[j] = 0;
	return (strings);
}
