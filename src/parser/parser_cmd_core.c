/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_core.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:45:55 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 12:48:28 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_cmd_node	*parse_simple_command(t_parser *parser)
{
	t_parse_simple_cmd	parse;
	t_token				*current;

	init_parse_simple_cmd_struct(&parse);
	current = parser->current_token;
	parse_simple_command_analyze_token(current, &parse, parser);
	if (parse.sflag)
	{
		parser->error = 1;
		advance_parser_position(parser);
		return (NULL);
	}
	parse_simple_command_loop_2(current, &parse, parser);
	parse.args[parse.i] = NULL;
	advance_parser_position(parser);
	parse.cmd = create_cmd_node(parser->exec, parse.args);
	if (!parse.cmd)
		return (parser->error = 1, NULL);
	set_command_redirections(parse.cmd, &parse, parser);
	return (parse.cmd);
}
