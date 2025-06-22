/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_lexer_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:45:55 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 12:48:28 by lyoussef         ###   ########.fr       */
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
