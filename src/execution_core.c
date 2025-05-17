/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_core.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:30:00 by marvin            #+#    #+#             */
/*   Updated: 2025/05/17 17:30:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_command_supreme(t_exec *exec, t_cmd_node *cmd)
{
	int	original_in;
	int	original_out;

	original_in = -1;
	original_out = -1;
	if (!exec || !cmd || !cmd->arr || !cmd->arr[0])
		return ;
	if (cmd->type == PIPE && handle_pipe_execution(exec))
		return ;
	if (is_builtin_command(cmd->arr[0])
		&& handle_builtin_without_redirect(exec, cmd))
		return ;
	execute_command_with_redirect(exec, cmd, original_in, original_out);
}

void	command_mission_control(t_cmd_node *cmd)
{
	if (!cmd || !cmd->exec)
		return ;
	execute_command_supreme(cmd->exec, cmd);
}

void	parse_and_execute(t_exec *exec, t_cmd_node *cmd)
{
	if (!exec || !cmd || !cmd->arr || !cmd->arr[0])
	{
		ft_putstr_fd("minishell: Error: Invalid command node\n", 2);
		exec->exit_status = 1;
		return ;
	}
	execute_command_supreme(exec, cmd);
}
