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
	int		consecutive_pipes;

	current = tokens;
	consecutive_pipes = 0;
	if (current && current->type == TOKEN_PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		exec->exit_status = 2;
		return (1);
	}
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			consecutive_pipes++;
			if (!current->next || current->next->type == TOKEN_EOF)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
				exec->exit_status = 2;
				return (1);
			}
			if (current->next && current->next->type == TOKEN_PIPE)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `||'\n", 2);
				exec->exit_status = 2;
				return (1);
			}
		}
		else
			consecutive_pipes = 0;
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
		if (current->type == TOKEN_REDIR_OUT || current->type == TOKEN_REDIR_APPEND
			|| current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_HEREDOC)
		{
			if (!current->next || current->next->type != TOKEN_WORD)
			{
				if (current->type == TOKEN_REDIR_OUT)
					ft_putstr_fd("minishell: syntax error near unexpected token `>'\n", 2);
				else if (current->type == TOKEN_REDIR_APPEND)
					ft_putstr_fd("minishell: syntax error near unexpected token `>>'\n", 2);
				else if (current->type == TOKEN_REDIR_IN)
					ft_putstr_fd("minishell: syntax error near unexpected token `<'\n", 2);
				else if (current->type == TOKEN_REDIR_HEREDOC)
					ft_putstr_fd("minishell: syntax error near unexpected token `<<'\n", 2);
				exec->exit_status = 2;
				return (1);
			}
			if (current->next && current->next->value &&
				ft_strlen(current->next->value) == 0)
			{
				if (current->type == TOKEN_REDIR_OUT)
					ft_putstr_fd("minishell: syntax error near unexpected token `>'\n", 2);
				else if (current->type == TOKEN_REDIR_APPEND)
					ft_putstr_fd("minishell: syntax error near unexpected token `>>'\n", 2);
				else if (current->type == TOKEN_REDIR_IN)
					ft_putstr_fd("minishell: syntax error near unexpected token `<'\n", 2);
				else if (current->type == TOKEN_REDIR_HEREDOC)
					ft_putstr_fd("minishell: syntax error near unexpected token `<<'\n", 2);
				exec->exit_status = 2;
				return (1);
			}
		}
		current = current->next;
	}
	return (0);
}

int	check_quote_syntax_errors(char *input, t_exec *exec)
{
	int		i;
	int		single_quote_open;
	int		double_quote_open;
	int		escaped;

	i = 0;
	single_quote_open = 0;
	double_quote_open = 0;
	escaped = 0;

	while (input[i])
	{
		if (escaped)
		{
			escaped = 0;
			i++;
			continue;
		}
		if (input[i] == '\\' && double_quote_open)
			escaped = 1;
		else if (input[i] == '\'' && !double_quote_open)
		{
			if (!single_quote_open)
				single_quote_open = 1;
			else
				single_quote_open = 0;
		}
		else if (input[i] == '"' && !single_quote_open)
		{
			if (!double_quote_open)
				double_quote_open = 1;
			else
				double_quote_open = 0;
		}
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
