/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_redirections.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:45:55 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 12:48:28 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	handle_cmd_redirection_token(t_token *current,
			t_parse_simple_cmd *parse, t_parser *parser)
{
	if ((current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_REDIR_APPEND) && parse->sflag == 0)
	{
		parse_simple_command_output(current, parse, parser);
		return (0);
	}
	else if (current->type == TOKEN_REDIR_IN)
	{
		if (parse_simple_command_input(current, parse, parser) == 1)
			return (0);
	}
	else if (current->type == TOKEN_REDIR_HEREDOC
		&& current->next && current->next->type == TOKEN_WORD)
	{
		if (parse_simple_command_heredoc(current, parse, parser) == 1)
			return (1);
	}
	return (0);
}

void	parse_simple_command_analyze_token(t_token *current,
		t_parse_simple_cmd *parse, t_parser *parser)
{
	while (current && current->type != TOKEN_EOF && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD)
			parse_simple_command_token_word(current, parse, parser);
		else if (handle_cmd_redirection_token(current, parse, parser) == 1)
		{
			parser->error = 1;
			parse->sflag = 1;
			break ;
		}
		current = current->next;
	}
}

void	set_command_redirections(t_cmd_node *cmd, t_parse_simple_cmd *parse,
		t_parser *parser)
{
	if (parse->last_input)
		cmd->in = ft_strdup(&parser->exec->gc, parse->last_input);
	if (parse->last_output)
	{
		cmd->out = ft_strdup(&parser->exec->gc, parse->last_output);
		cmd->append = parse->last_output_is_append;
	}
	if (parse->heredoc_delimiter)
	{
		cmd->heredoc_delimiter = parse->heredoc_delimiter;
		parse->heredoc_delimiter = NULL;
	}
}

t_cmd_node	*parse_simple_command(t_parser *parser)
{
	t_parse_simple_cmd	parse;
	t_token				*current;

	init_parse_simple_cmd_struct(&parse);
	current = parser->current_token;
	parse_simple_command_analyze_token(current, &parse, parser);
	if (parse.sflag)
	{
		parser->error = 1;
		advance_parser_position(parser);
		return (NULL);
	}
	parse_simple_command_loop_2(current, &parse, parser);
	parse.args[parse.i] = NULL;
	advance_parser_position(parser);
	parse.cmd = create_cmd_node(parser->exec, parse.args);
	if (!parse.cmd)
		return (parser->error = 1, NULL);
	set_command_redirections(parse.cmd, &parse, parser);
	return (parse.cmd);
}
