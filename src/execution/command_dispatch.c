/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_dispatch.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 09:18:08 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	execute_builtin_command(t_exec *exec, t_cmd_node *cmd)
{
	return (handle_builtin_command(exec, cmd));
}

int	execute_external_command(t_exec *exec, t_cmd_node *cmd)
{
	char	*cmd_path;
	char	**env_array;
	char	*expanded_cmd;

	if (prep_cmd_for_launch(exec, cmd, &expanded_cmd, &cmd_path))
		return (1);
	if (handle_heredoc(cmd, exec) == -1)
	{
		exec->exit_status = 1;
		return (1);
	}
	if (prepare_process_environment(exec, &env_array))
		return (1);
	return (create_and_execute_process(exec, cmd, cmd_path, env_array));
}

int	handle_pipe_execution(t_exec *exec)
{
	execute_with_pipes(exec, exec->cmd_list);
	return (1);
}

int	handle_builtin_without_redirect(t_exec *exec, t_cmd_node *cmd)
{
	if (!cmd->in && !cmd->out)
	{
		execute_builtin_command(exec, cmd);
		return (1);
	}
	return (0);
}

int	execute_command_with_redirect(t_exec *exec, t_cmd_node *cmd,
	int original_in, int original_out)
{
	if (setup_redirections(cmd, &original_in, &original_out) == -1)
		return (1);
	if (!cmd->arr[0])
	{
		restore_og_redirections(original_in, original_out);
		return (0);
	}
	if (is_builtin_command(cmd->arr[0]))
		execute_builtin_command(exec, cmd);
	else
		execute_external_command(exec, cmd);
	restore_og_redirections(original_in, original_out);
	return (0);
}
