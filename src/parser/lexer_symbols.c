/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 00:46:31 by marvin            #+#    #+#             */
/*   Updated: 2025/06/02 00:46:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_token	*get_next_token_pipe(t_lexer *lexer, t_gc *gc, char *current)
{
	if ((*current) == '|')
	{
		lexer->position++;
		if (lexer->position == 1 || !lexer->input[lexer->position]
			|| lexer->input[lexer->position] == '|')
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n",
				2);
			lexer->error = 1;
			return (create_token(TOKEN_EOF, NULL, gc));
		}
		return (create_token(TOKEN_PIPE, "|", gc));
	}
	return (NULL);
}

t_token	*input_error(t_lexer *lexer, t_gc *gc)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `<<<'\n", 2);
	lexer->error = 1;
	return (create_token(TOKEN_EOF, NULL, gc));
}

t_token	*get_next_token_input_r(t_lexer *lexer, t_gc *gc, char *current)
{
	if ((*current) == '<')
	{
		lexer->position++;
		if (lexer->input[lexer->position] == '<')
		{
			lexer->position++;
			if (lexer->input[lexer->position] == '<')
				return (input_error(lexer, gc));
			skip_whitespace(lexer);
			if (!lexer->input[lexer->position]
				|| lexer->input[lexer->position] == '|'
				|| lexer->input[lexer->position] == '<'
				|| lexer->input[lexer->position] == '>')
				return (input_error(lexer, gc));
			return (create_token(TOKEN_REDIR_HEREDOC, "<<", gc));
		}
		skip_whitespace(lexer);
		if (!lexer->input[lexer->position]
			|| lexer->input[lexer->position] == '|'
			|| lexer->input[lexer->position] == '<'
			|| lexer->input[lexer->position] == '>')
			return (input_error(lexer, gc));
		return (create_token(TOKEN_REDIR_IN, "<", gc));
	}
	return (NULL);
}

t_token	*get_next_token_output(t_lexer *lexer, t_gc *gc, char *current)
{
	if ((*current) == '>')
	{
		lexer->position++;
		if (lexer->input[lexer->position] == '>')
		{
			lexer->position++;
			return (create_token(TOKEN_REDIR_APPEND, ">>", gc));
		}
		return (create_token(TOKEN_REDIR_OUT, ">", gc));
	}
	return (NULL);
}
