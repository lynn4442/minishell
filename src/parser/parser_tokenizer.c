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

t_token	*tokenize(char *input, t_gc *gc)
{
	t_lexer	*lexer;
	t_token	*token_list;
	t_token	*token;
	char	*copied_input;

	copied_input = copy_input_string(input, gc);
	lexer = init_lexer(copied_input, gc);
	if (!lexer)
		return (NULL);
	token_list = NULL;
	while (1)
	{
		token = get_next_token(lexer, gc);
		if (!token)
			return (NULL);
		add_token(&token_list, token);
		if (token->type == TOKEN_EOF || lexer->error)
			break ;
	}
	if (lexer->error)
		return (NULL);
	return (token_list);
}
