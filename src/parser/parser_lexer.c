/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_lexer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 23:52:02 by marvin            #+#    #+#             */
/*   Updated: 2025/06/01 23:52:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

void	skip_whitespace(t_lexer *lexer)
{
	while (lexer->input[lexer->position]
		&& is_whitespace(lexer->input[lexer->position]))
		lexer->position++;
}

t_token	*get_next_token(t_lexer *lexer, t_gc *gc)
{
	char	current;
	t_token	*word_token;
	t_token	*temp;

	skip_whitespace(lexer);
	if (!lexer->input[lexer->position])
		return (create_token(TOKEN_EOF, NULL, gc));
	current = lexer->input[lexer->position];
	temp = get_next_token_pipe(lexer, gc, &current);
	if (temp != NULL)
		return (temp);
	temp = get_next_token_input_r(lexer, gc, &current);
	if (temp != NULL)
		return (temp);
	temp = get_next_token_output(lexer, gc, &current);
	if (temp != NULL)
		return (temp);
	word_token = get_word_token(lexer, gc);
	if (word_token)
		return (word_token);
	lexer->position++;
	return (get_next_token(lexer, gc));
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
