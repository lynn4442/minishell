/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhussein <hhussein@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/02 22:01:54 by hhussein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipes.h"

void	handle_quote_state(char c, char *nested_quote, int *j)
{
	if ((c == '\'' || c == '"') && *nested_quote == 0)
	{
		*nested_quote = c;
		(*j)++;
	}
	else if (c == *nested_quote)
	{
		*nested_quote = 0;
		(*j)++;
	}
}

int	handle_escape_state(const char *arg, int *j, int *escaped, char quote)
{
	if (arg[*j] == '\\' && *escaped == 0 && (quote == 0 || quote == '"'))
	{
		*escaped = 1;
		(*j)++;
		return (1);
	}
	return (0);
}

char	*process_argument(const char *arg, t_gc *gc)
{
	char			*processed;
	t_r_variables	var;

	init_r_variables(&var);
	processed = ft_malloc(gc, ft_strlen(arg) + 1);
	if (!processed)
		return (NULL);
	while (arg[var.j])
	{
		if ((arg[var.j] == '\'' || arg[var.j] == '"') && var.escaped == 0)
		{
			handle_quote_state(arg[var.j], &var.quote, &var.j);
			continue ;
		}
		if (handle_escape_state(arg, &var.j, &var.escaped, var.quote))
			continue ;
		processed[var.k++] = arg[var.j++];
		var.escaped = 0;
	}
	processed[var.k] = '\0';
	return (processed);
}

static char	*extract_argument(const char *input, t_r_variables *var,
	t_gc *gc, int is_echo_command)
{
	int		start;
	char	*arg;
	char	*processed;

	skip_leading_spaces(input, &var->i, &var->quote);
	if (!input[var->i])
		return (NULL);
	if (!(var->quote) && !(var->escaped) && (input
			[var->i] == '>' || input[var->i] == '<'))
		return (extract_redirection(input, &var->i, gc));
	start = var->i;
	extract_argument_bounds(input, &var->i, &var->quote, &var->escaped);
	if (start >= var->i)
		return (NULL);
	arg = ft_strndup(gc, input + start, var->i - start);
	if (!arg)
		return (NULL);
	if (!is_echo_command)
	{
		processed = process_argument(arg, gc);
		if (processed)
			return (processed);
	}
	return (arg);
}

char	**split_preserve_quotes(const char *input, t_gc *gc)
{
	char			**result;
	t_r_variables	var;
	int				arg_count;
	char			*arg;

	init_r_variables(&var);
	arg_count = count_arguments(input, gc);
	result = ft_malloc(gc, sizeof(char *) * (arg_count + 1));
	if (!result)
		return (NULL);
	while (var.count < arg_count)
	{
		arg = extract_argument(input, &var, gc, var.is_echo_command);
		if (!arg)
			break ;
		if (var.count == 0 && ft_strcmp(arg, "echo") == 0)
			var.is_echo_command = 1;
		result[var.count++] = arg;
		if (!input[var.i])
			break ;
		var.i++;
	}
	result[var.count] = NULL;
	return (result);
}
