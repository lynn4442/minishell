/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wasmar <wasmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 00:00:00 by wasmar            #+#    #+#             */
/*   Updated: 2025/01/11 00:00:00 by wasmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	init_parse_simple_cmd_struct(t_parse_simple_cmd *parse)
{
	parse->arg_count = 0;
	parse->i = 0;
	parse->filename = NULL;
	parse->last_input = NULL;
	parse->last_output = NULL;
	parse->sflag = 0;
	parse->last_output_is_append = 0;
	//parse->heredoc_delimiter = NULL; heyda l part li zedto
}

void	parse_simple_command_token_word(t_token *current,
		t_parse_simple_cmd *parse, t_parser *parser)
{
	if (!(current != parser->current_token
			&& (current->prev->type == TOKEN_REDIR_IN
				|| current->prev->type == TOKEN_REDIR_OUT
				|| current->prev->type == TOKEN_REDIR_APPEND
				|| current->prev->type == TOKEN_REDIR_HEREDOC)))
	{
		parse->arg_count++;
	}
}

void	parse_simple_command_analyze_token(t_token *current,
		t_parse_simple_cmd *parse, t_parser *parser)
{
	while (current && current->type != TOKEN_EOF && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD)
			parse_simple_command_token_word(current, parse, parser);
		else if ((current->type == TOKEN_REDIR_OUT
				|| current->type == TOKEN_REDIR_APPEND) && parse->sflag == 0)
			parse_simple_command_output(current, parse, parser);
		else if (current->type == TOKEN_REDIR_IN)
		{
			if (parse_simple_command_input(current, parse, parser) == 1)
			{
				current = current->next;
				continue ;
			}
		}
		// heyda l part li zedto
		//else if (current->type == TOKEN_REDIR_HEREDOC && current->next)
		//{
		//	parse->heredoc_delimiter = current->next->value;
		//	current = current->next->next;
		//}
		current = current->next;
	}
}

void	parse_simple_command_loop_2(t_token *current,
		t_parse_simple_cmd *parse, t_parser *parser)
{
	parse->args = ft_malloc(&parser->exec->gc, sizeof(char *)
			* (parse->arg_count + 1));
	current = parser->current_token;
	while (current && current->type != TOKEN_EOF && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD)
		{
			if (!(current != parser->current_token
					&& (current->prev->type == TOKEN_REDIR_IN
						|| current->prev->type == TOKEN_REDIR_OUT
						|| current->prev->type == TOKEN_REDIR_APPEND
						|| current->prev->type == TOKEN_REDIR_HEREDOC)))
			{
				parse->args[parse->i++] = ft_strdup(&parser->exec->gc,
						current->value);
			}
		}
		current = current->next;
	}
}

t_cmd_node	*parse_simple_command(t_parser *parser)
{
	t_parse_simple_cmd	parse;
	t_token				*current;

	init_parse_simple_cmd_struct(&parse);
	current = parser->current_token;
	parse_simple_command_analyze_token(current, &parse, parser);
	parse_simple_command_loop_2(current, &parse, parser);
	parse.args[parse.i] = NULL;
	advance_parser_position(parser);
	parse.cmd = create_cmd_node(parser->exec, parse.args);
	if (!parse.cmd)
		return (parser->error = 1, NULL);
	if (parse.last_input)
		parse.cmd->in = ft_strdup(&parser->exec->gc, parse.last_input);
	if (parse.last_output)
	{
		parse.cmd->out = ft_strdup(&parser->exec->gc, parse.last_output);
		parse.cmd->append = parse.last_output_is_append;
	}
	// heyda l part li zedto
	//if (parse.heredoc_delimiter)
	//{
	//	parse.cmd->heredoc = ft_strdup(&parser->exec->gc,
	//			parse.heredoc_delimiter);
	//}
	return (parse.cmd);
}
