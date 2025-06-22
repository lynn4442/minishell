/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipeline_redirections.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:45:55 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 12:48:28 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// > and >>
static void	handle_pipeline_output_redirection(t_parse_pipeline *parse,
			t_parser *parser)
{
	parse->is_append = (parser->current_token->type == TOKEN_REDIR_APPEND);
	advance_token(parser);
	parse_pipeline_token_word(parse, parser);
}

// <
static int	handle_pipeline_input_redirection(t_parse_pipeline *parse,
			t_parser *parser)
{
	(void)parse;
	advance_token(parser);
	if (!parser->current_token || parser->current_token->type != TOKEN_WORD)
	{
		parser->error = 1;
		return (1);
	}
	return (0);
}

// << 
static int	handle_pipeline_heredoc_redirection(t_parse_pipeline *parse,
			t_parser *parser)
{
	(void)parse;
	advance_token(parser);
	if (!parser->current_token || parser->current_token->type != TOKEN_WORD)
	{
		parser->error = 1;
		return (1);
	}
	return (0);
}

/* Process redirection tokens in pipeline */
int	process_redirection_token(t_parse_pipeline *parse, t_parser *parser)
{
	if (parser->current_token->type == TOKEN_REDIR_OUT
		|| parser->current_token->type == TOKEN_REDIR_APPEND)
	{
		handle_pipeline_output_redirection(parse, parser);
	}
	else if (parser->current_token->type == TOKEN_REDIR_IN)
	{
		if (handle_pipeline_input_redirection(parse, parser) == 1)
			return (1);
	}
	else if (parser->current_token->type == TOKEN_REDIR_HEREDOC)
	{
		if (handle_pipeline_heredoc_redirection(parse, parser) == 1)
			return (1);
	}
	return (0);
}
