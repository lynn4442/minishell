/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error_handling_helper.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 20:14:51 by hhussein          #+#    #+#             */
/*   Updated: 2025/06/26 14:57:57 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_invalid_pipe(t_token *current, t_exec *exec)
{
	if (!current->next || current->next->type == TOKEN_EOF)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		exec->exit_status = 2;
		return (1);
	}
	if (current->next->type == TOKEN_PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `||'\n", 2);
		exec->exit_status = 2;
		return (1);
	}
	return (0);
}

int	print_redirection_error(t_token_type type, t_exec *exec)
{
	const char	*msg;

	msg = NULL;
	if (type == TOKEN_REDIR_OUT)
		msg = "minishell: syntax error near unexpected token `>'\n";
	else if (type == TOKEN_REDIR_APPEND)
		msg = "minishell: syntax error near unexpected token `>>'\n";
	else if (type == TOKEN_REDIR_IN)
		msg = "minishell: syntax error near unexpected token `<'\n";
	else if (type == TOKEN_REDIR_HEREDOC)
		msg = "minishell: syntax error near unexpected token `<<'\n";
	if (msg)
		ft_putstr_fd(msg, 2);
	exec->exit_status = 2;
	return (1);
}

void	update_quote_state(char c, int *single_q, int *double_q, int *escaped)
{
	if (*escaped)
	{
		*escaped = 0;
		return ;
	}
	if (c == '\\' && *double_q && !*single_q)
		*escaped = 1;
	else if (c == '\'' && !*double_q)
		*single_q = !*single_q;
	else if (c == '"' && !*single_q)
		*double_q = !*double_q;
}

char	*process_quoted_token(char *val, t_parser *parser)
{
	return (process_quotes(val, parser->exec->env_list, parser->exec));
}
