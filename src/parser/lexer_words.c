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

void	init_get_word_token_struct(t_get_word_token *norm, t_lexer *lexer)
{
	norm->i = lexer->position;
	norm->in_quotes = 0;
	norm->quote_type = 0;
	norm->escaped = 0;
	norm->j = 0;
}

int	get_word_token_quotes(t_lexer *lexer, t_get_word_token *norm)
{
	if ((lexer->input[norm->i] == '\'' || lexer->input[norm->i] == '"')
		&& !norm->escaped)
	{
		if (!norm->in_quotes)
		{
			norm->in_quotes = 1;
			norm->quote_type = lexer->input[norm->i];
			norm->temp[norm->j++] = lexer->input[norm->i++];
			return (1);
		}
		else if (lexer->input[norm->i] == norm->quote_type)
		{
			norm->in_quotes = 0;
			norm->quote_type = 0;
			norm->temp[norm->j++] = lexer->input[norm->i++];
			return (1);
		}
	}
	return (0);
}

int	get_word_token_escaped(t_lexer *lexer, t_get_word_token *norm)
{
	if (lexer->input[norm->i] == '\\' && !norm->escaped)
	{
		norm->escaped = 1;
		norm->temp[norm->j++] = lexer->input[norm->i++];
		return (1);
	}
	if (norm->escaped)
	{
		norm->temp[norm->j++] = lexer->input[norm->i++];
		norm->escaped = 0;
		return (1);
	}
	return (0);
}

int	get_word_token_testcase(t_lexer *lexer, t_get_word_token *norm)
{
	if (!norm->in_quotes && (ft_isspace(lexer->input[norm->i])
			|| lexer->input[norm->i] == '|' || lexer->input[norm->i] == '>'
			|| lexer->input[norm->i] == '<'))
	{
		return (0);
	}
	return (1);
}

t_token	*get_word_token(t_lexer *lexer, t_gc *gc)
{
	t_get_word_token	norm;

	init_get_word_token_struct(&norm, lexer);
	if (lexer->input[norm.i] == '>' || lexer->input[norm.i] == '<')
		return (NULL);
	norm.temp = ft_malloc(gc, ft_strlen(lexer->input) + 1);
	while (lexer->input[norm.i])
	{
		if (get_word_token_quotes(lexer, &norm) == 1)
			continue ;
		if (get_word_token_escaped(lexer, &norm) == 1)
			continue ;
		if (get_word_token_testcase(lexer, &norm) == 0)
			break ;
		norm.temp[norm.j++] = lexer->input[norm.i++];
	}
	norm.temp[norm.j] = '\0';
	if (norm.j > 0)
	{
		norm.word = ft_strdup(gc, norm.temp);
		lexer->position = norm.i;
		return (create_token(TOKEN_WORD, norm.word, gc));
	}
	return (NULL);
}
