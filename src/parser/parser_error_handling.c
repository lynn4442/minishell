/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error_handling.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/01/20 10:00:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	check_pipe_syntax_errors(t_token *tokens, t_exec *exec)
{
	t_token	*current;

	if (tokens && tokens->type == TOKEN_PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		exec->exit_status = 2;
		return (1);
	}
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (is_invalid_pipe(current, exec))
				return (1);
		}
		current = current->next;
	}
	return (0);
}

int	check_redirection_syntax_errors(t_token *tokens, t_exec *exec)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_REDIR_APPEND
			|| current->type == TOKEN_REDIR_IN
			|| current->type == TOKEN_REDIR_HEREDOC)
		{
			if (!current->next
				|| current->next->type != TOKEN_WORD
				|| ft_strlen(current->next->value) == 0)
				return (print_redirection_error(current->type, exec));
		}
		current = current->next;
	}
	return (0);
}

int	check_quote_syntax_errors(char *input, t_exec *exec)
{
	int	i;
	int	single_quote_open;
	int	double_quote_open;
	int	escaped;

	i = 0;
	single_quote_open = 0;
	double_quote_open = 0;
	escaped = 0;
	while (input[i])
	{
		update_quote_state(input[i],
			&single_quote_open, &double_quote_open, &escaped);
		i++;
	}
	if (single_quote_open || double_quote_open)
	{
		ft_putstr_fd("minishell: syntax error: unclosed quotes\n", 2);
		exec->exit_status = 2;
		return (1);
	}
	return (0);
}

int	validate_syntax(char *input, t_token *tokens, t_exec *exec)
{
	if (!input || !*input)
		return (0);
	if (check_quote_syntax_errors(input, exec))
		return (1);
	if (check_pipe_syntax_errors(tokens, exec))
		return (1);
	if (check_redirection_syntax_errors(tokens, exec))
		return (1);
	return (0);
}
