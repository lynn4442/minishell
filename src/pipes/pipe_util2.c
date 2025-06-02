/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_util2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhussein <hhussein@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 20:54:20 by hhussein          #+#    #+#             */
/*   Updated: 2025/06/02 21:59:01 by hhussein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipes.h"

void	handle_escape_and_quotes(char c, int *escaped, char *quote)
{
	if (c == '\\' && !(*escaped))
	{
		*escaped = 1;
		return ;
	}
	if (*escaped)
	{
		*escaped = 0;
		return ;
	}
	if (c == '\'' || c == '"')
	{
		if (!(*quote))
			*quote = c;
		else if (*quote == c)
			*quote = 0;
	}
}

void	skip_space(const char *input, int *i)
{
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
}

static	int	is_redirection(char c)
{
	return (c == '<' || c == '>');
}

int	handle_redirection_pipe(const char *input, int *i, int *count)
{
	if (!is_redirection(input[*i]))
		return (0);
	(*count)++;
	if (input[*i + 1] && input[*i] == input[*i + 1])
		*i += 2;
	else
		(*i)++;
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	return (1);
}

void	skip_argument_token(const char *input,
	int *i, int *escaped, char *quote)
{
	while (input[*i])
	{
		handle_escape_and_quotes(input[*i], escaped, quote);
		if (*escaped)
		{
			(*i)++;
			continue ;
		}
		if (!*quote && (ft_isspace(input[*i]) || is_redirection(input[*i])))
			break ;
		(*i)++;
	}
}
