/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wasmar <wasmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 00:00:00 by wasmar            #+#    #+#             */
/*   Updated: 2025/01/11 00:00:00 by wasmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	init_parse_pipeline(t_parse_pipeline *parse, t_parser *parser)
{
	parse->pipe_count = 0;
	parse->sflag = 0;
	parse->fd = 0;
	parse->start_token = parser->current_token;
	parse->is_append = 0;
}

void	parse_pipeline_token_word(t_parse_pipeline *parse, t_parser *parser)
{
	char	*filename;
	char	*unquoted;

	if (parser->current_token && parser->current_token->type == TOKEN_WORD)
	{
		filename = parser->current_token->value;
		if (filename[0] == '"' && filename[ft_strlen(filename) - 1] == '"')
		{
			unquoted = ft_substr(filename, 1, ft_strlen(filename) - 2);
			if (unquoted)
			{
				filename = ft_strdup(&parser->exec->gc, unquoted);
				free(unquoted);
			}
		}
		if (parse->is_append)
			parse->fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			parse->fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (parse->fd >= 0)
			close(parse->fd);
		else
			perror(filename);
	}
}

void	pipe_file_creation(t_parse_pipeline *parse, t_parser *parser)
{
	while (parser->current_token && parser->current_token->type != TOKEN_EOF
		&& parse->sflag == 0)
	{
		if (parser->current_token->type == TOKEN_REDIR_OUT
			|| parser->current_token->type == TOKEN_REDIR_APPEND)
		{
			parse->is_append = (parser->current_token->type
					== TOKEN_REDIR_APPEND);
			advance_token(parser);
			parse_pipeline_token_word(parse, parser);
		}
		else if (parser->current_token->type == TOKEN_REDIR_IN)
		{
			parse->fd = open(parser->current_token->value, O_RDONLY);
			if (parse->fd < 0)
				parse->sflag = 1;
		}
		advance_token(parser);
	}
}

t_cmd_node	*parse_pipeline(t_parser *parser)
{
	t_parse_pipeline	parse;

	init_parse_pipeline(&parse, parser);
	pipe_file_creation(&parse, parser);
	parser->current_token = parse.start_token;
	parse.first_cmd = parse_simple_command(parser);
	if (!parse.first_cmd || parser->error)
		return (NULL);
	parse.current_cmd = parse.first_cmd;
	while (parser->current_token && parser->current_token->type == TOKEN_PIPE)
	{
		parse.pipe_count++;
		advance_token(parser);
		parse.current_cmd->type = PIPE;
		parse.next_cmd = parse_simple_command(parser);
		if (parser->error == 1)
			return (parse.first_cmd);
		parse.current_cmd->next = parse.next_cmd;
		parse.current_cmd = parse.next_cmd;
	}
	return (parse.first_cmd);
}
