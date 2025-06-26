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

static t_token	*handle_redirection_tokens(t_lexer *lexer, t_gc *gc,
				char current)
{
	if (current == '<')
	{
		lexer->position++;
		if (lexer->input[lexer->position] == '<')
		{
			lexer->position++;
			return (create_token(TOKEN_REDIR_HEREDOC, "<<", gc));
		}
		return (create_token(TOKEN_REDIR_IN, "<", gc));
	}
	if (current == '>')
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

t_token	*get_next_token(t_lexer *lexer, t_gc *gc)
{
	char	current;
	t_token	*redir_token;

	skip_whitespace(lexer);
	if (!lexer->input[lexer->position])
		return (create_token(TOKEN_EOF, NULL, gc));
	current = lexer->input[lexer->position];
	redir_token = handle_redirection_tokens(lexer, gc, current);
	if (redir_token)
		return (redir_token);
	if (current == '|')
	{
		lexer->position++;
		return (create_token(TOKEN_PIPE, "|", gc));
	}
	return (get_word_token(lexer, gc));
}

t_lexer	*init_lexer(char *input, t_gc *gc)
{
	t_lexer	*lexer;

	lexer = ft_malloc(gc, sizeof(t_lexer));
	if (!lexer)
		return (NULL);
	lexer->input = input;
	lexer->position = 0;
	lexer->gc = gc;
	lexer->error = 0;
	return (lexer);
}

void	skip_whitespace(t_lexer *lexer)
{
	while (lexer->input[lexer->position]
		&& ft_isspace(lexer->input[lexer->position]))
		lexer->position++;
}

char	*copy_input_string(char *input, t_gc *gc)
{
	int		i;
	int		j;
	int		len;
	char	*copied;

	i = 0;
	j = 0;
	len = ft_strlen(input);
	copied = ft_malloc(gc, len + 1);
	if (!copied)
		return (input);
	while (input[i])
	{
		copied[j++] = input[i++];
	}
	copied[j] = '\0';
	return (copied);
}
