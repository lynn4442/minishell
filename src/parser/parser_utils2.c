/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_interface.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wasmar <wasmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 00:00:00 by wasmar            #+#    #+#             */
/*   Updated: 2025/01/11 00:00:00 by wasmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_cmd_node	*parse_piped_commands(char *input, t_exec *exec)
{
	return (parse_input(input, exec));
}

/* Main parse function for external call */
t_cmd_node	*parse_command_line(char *input, t_exec *exec)
{
	t_cmd_node	*cmd_list;

	cmd_list = parse_input(input, exec);
	if (cmd_list)
	{
		if (check_syntax(cmd_list, exec))
		{
			exec->cmd_list = NULL;
			return (NULL);
		}
		exec->cmd_list = cmd_list;
	}
	return (cmd_list);
}
