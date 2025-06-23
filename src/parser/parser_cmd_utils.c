/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:45:55 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 12:48:28 by lyoussef         ###   ########.fr       */
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
	parse->heredoc_delimiter = NULL;
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

static int	should_skip_token(t_token *current, t_parser *parser)
{
	return (current != parser->current_token
		&& (current->prev->type == TOKEN_REDIR_IN
			|| current->prev->type == TOKEN_REDIR_OUT
			|| current->prev->type == TOKEN_REDIR_APPEND
			|| current->prev->type == TOKEN_REDIR_HEREDOC));
}

static void	process_word_token(t_token *current, t_parse_simple_cmd *parse,
		t_parser *parser)
{
	if (!should_skip_token(current, parser))
	{
		parse->args[parse->i++] = process_quoted_token(current->value, parser);
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
			process_word_token(current, parse, parser);
		current = current->next;
	}
}

/* Moved from parser_cmd_quotes.c */
char	*process_quoted_token(char *val, t_parser *parser)
{
	// Don't do any quote processing here - leave it to the commands
	return (ft_strdup(&parser->exec->gc, val));
}
