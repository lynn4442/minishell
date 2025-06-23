/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:45:55 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 12:48:28 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_parser	*init_parser(t_token *tokens, t_exec *exec)
{
	t_parser	*parser;

	parser = ft_malloc(&exec->gc, sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->current_token = tokens;
	parser->token_list = tokens;
	parser->exec = exec;
	parser->error = 0;
	return (parser);
}

void	advance_token(t_parser *parser)
{
	if (parser->current_token && parser->current_token->type != TOKEN_EOF)
		parser->current_token = parser->current_token->next;
}

void	advance_parser_position(t_parser *parser)
{
	while (parser->current_token && parser->current_token->type != TOKEN_EOF
		&& parser->current_token->type != TOKEN_PIPE)
	{
		advance_token(parser);
	}
}

t_cmd_node	*parse_input(char *input, t_exec *exec)
{
	t_token		*tokens;
	t_parser	*parser;
	t_cmd_node	*cmd_list;

	if (!input || input[0] == '\0')
		return (NULL);
	if (!check_quotes(input))
	{
		ft_putstr_fd("minishell: syntax error: unclosed quotes\n", 2);
		exec->exit_status = 2;
		return (NULL);
	}
	tokens = tokenize(input, &exec->gc);
	if (!tokens)
		return (NULL);
	if (validate_syntax(input, tokens, exec))
		return (NULL);
	parser = init_parser(tokens, exec);
	if (!parser)
		return (NULL);
	cmd_list = parse_pipeline(parser);
	if (parser->error && exec->exit_status == 0 && !parser->current_token)
	{
		ft_putstr_fd("minishell: syntax error\n", 2);
		exec->exit_status = 2;
	}
	return (cmd_list);
}

int	check_syntax(t_cmd_node *cmd_list, t_exec *exec)
{
	t_cmd_node	*current;

	if (!cmd_list)
		return (0);
	current = cmd_list;
	while (current)
	{
		if (!current->arr)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
			exec->exit_status = 2;
			return (1);
		}
		if (current->type == PIPE && !current->next)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n",
				2);
			exec->exit_status = 2;
			return (1);
		}
		current = current->next;
	}
	return (0);
}

/* Moved from parser_utils2.c */
t_cmd_node	*parse_piped_commands(char *input, t_exec *exec)
{
	return (parse_input(input, exec));
}

/* Main parse function for external call */
t_cmd_node	*parse_command_line(char *input, t_exec *exec)
{
	t_cmd_node	*cmd_list;

	cmd_list = parse_input(input, exec);
	if (cmd_list)
	{
		if (check_syntax(cmd_list, exec))
		{
			exec->cmd_list = NULL;
			return (NULL);
		}
		exec->cmd_list = cmd_list;
	}
	return (cmd_list);
}
