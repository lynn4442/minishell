/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argument_count.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhussein <hhussein@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:41:05 by hhussein          #+#    #+#             */
/*   Updated: 2025/05/28 19:54:29 by hhussein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	skip_spaces(const char *input, t_argument_count *arg)
{
	while (input[arg->i] && ft_isspace(input[arg->i]) && !arg->quote)
		arg->i++;
}

static int	handle_redirect(const char *input, t_argument_count *arg)
{
	if (!arg->quote && !arg->escaped
		&& (input[arg->i] == '>' || input[arg->i] == '<'))
	{
		arg->count++;
		if (input[arg->i + 1] && input[arg->i] == input[arg->i + 1])
			arg->i += 2;
		else
			arg->i++;
		while (input[arg->i] && ft_isspace(input[arg->i]))
			arg->i++;
		return (1);
	}
	return (0);
}

static void	skip_token(const char *input, t_argument_count *arg)
{
	while (input[arg->i])
	{
		handle_escape_and_quotes(input[arg->i], &arg->escaped, &arg->quote);
		if (arg->escaped)
		{
			arg->i++;
			continue ;
		}
		if (ft_isspace(input[arg->i]) && !arg->quote)
			break ;
		if (!arg->quote && !arg->escaped && (input
				[arg->i] == '>' || input[arg->i] == '<'))
			break ;
		arg->i++;
	}
}

void	set_value(t_argument_count *arg)
{
	arg->i = 0;
	arg->count = 0;
	arg->quote = 0;
	arg->escaped = 0;
	arg->start = 0;
}

int	count_arguments(const char *input, t_gc *gc)
{
	t_argument_count	arg;

	(void)gc;
	set_value(&arg);
	while (input[arg.i])
	{
		skip_spaces(input, &arg);
		if (!input[arg.i])
			break ;
		arg.start = arg.i;
		if (handle_redirect(input, &arg))
		{
			if (!input[arg.i])
				break ;
			continue ;
		}
		skip_token(input, &arg);
		if (arg.start < arg.i)
			arg.count++;
		if (!input[arg.i])
			break ;
		if (ft_isspace(input[arg.i]))
			arg.i++;
	}
	return (arg.count);
}
