/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_util3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhussein <hhussein@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 20:56:13 by hhussein          #+#    #+#             */
/*   Updated: 2025/06/02 21:58:50 by hhussein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipes.h"

void	init_r_variables(t_r_variables *vars)
{
	if (!vars)
		return ;
	vars->i = 0;
	vars->j = 0;
	vars->k = 0;
	vars->escaped = 0;
	vars->quote = 0;
	vars->count = 0;
	vars->is_echo_command = 0;
}

int	count_arguments(const char *input, t_gc *gc)
{
	t_r_variables	var;
	int				start;

	init_r_variables(&var);
	(void)gc;
	while (input[var.i])
	{
		skip_space(input, &var.i);
		if (!input[var.i])
			break ;
		start = var.i;
		if (handle_redirection_pipe(input, &var.i, &var.count))
		{
			if (!input[var.i])
				break ;
			continue ;
		}
		skip_argument_token(input, &var.i, &var.escaped, &var.quote);
		if (start < var.i)
			var.count++;
		if (input[var.i] && ft_isspace(input[var.i]))
			var.i++;
	}
	return (var.count);
}

void	skip_leading_spaces(const char *input, int *i, char *quote)
{
	while (input[*i] && ft_isspace(input[*i]) && !(*quote))
		(*i)++;
}

char	*extract_redirection(const char *input, int *i, t_gc *gc)
{
	int	start;

	start = *i;
	if (input[*i + 1] && input[*i] == input[*i + 1])
	{
		*i += 2;
		return (ft_strndup(gc, input + start, 2));
	}
	else
	{
		(*i)++;
		return (ft_strndup(gc, input + start, 1));
	}
}

void	extract_argument_bounds(const char *input, int *i,
	char *quote, int *escaped)
{
	while (input[*i])
	{
		handle_escape_and_quotes(input[*i], escaped, quote);
		if (*escaped)
		{
			(*i)++;
			continue ;
		}
		if (ft_isspace(input[*i]) && !(*quote))
			break ;
		if (!(*quote) && !(*escaped) && (input[*i] == '>' || input[*i] == '<'))
			break ;
		(*i)++;
	}
}
