/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhussein <hhussein@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 20:27:23 by hhussein          #+#    #+#             */
/*   Updated: 2025/06/25 20:31:19 by hhussein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_cmd_node	*parse_and_validate(char *input, t_exec *exec)
{
	t_token		*tokens;
	t_parser	*parser;

	tokens = tokenize(input, &exec->gc);
	if (!tokens)
		return (NULL);
	if (validate_syntax(input, tokens, exec))
		return (NULL);
	parser = init_parser(tokens, exec);
	if (!parser)
		return (NULL);
	if (parser->error && exec->exit_status == 0 && !parser->current_token)
	{
		ft_putstr_fd("minishell: syntax error\n", 2);
		exec->exit_status = 2;
		return (NULL);
	}
	return (parse_pipeline(parser));
}

t_cmd_node	*parse_input(char *input, t_exec *exec)
{
	if (!input || input[0] == '\0')
		return (NULL);
	if (!check_quotes(input))
	{
		ft_putstr_fd("minishell: syntax error: unclosed quotes\n", 2);
		exec->exit_status = 2;
		return (NULL);
	}
	return (parse_and_validate(input, exec));
}
