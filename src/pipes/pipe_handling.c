/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/25 15:30:28 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipes.h"

static int	init_pipes_and_env(t_exec *exec, int pipe_count,
	int pipes[][2], char ***env_array)
{
	if (!create_all_pipes(pipe_count, pipes))
		return (0);
	*env_array = convert_env_to_array(exec, &exec->gc);
	if (!*env_array)
	{
		ft_putstr_fd("minishell: environment conversion failed\n", 2);
		close_all_pipes(pipe_count, pipes);
		return (0);
	}
	return (1);
}

void	execute_with_pipes(t_exec *exec, t_cmd_node *cmd_list)
{
	t_r_variables	var;
	char			**env_array;
	t_cmd_node		*cmd;
	int				heredoc_result;

	var.cmd_count = count_pipeline_commands(cmd_list);
	if (var.cmd_count <= 0)
		return ;
	cmd = cmd_list;
	while (cmd)
	{
		if (cmd->heredoc_delimiter)
		{
			heredoc_result = handle_heredoc(cmd, exec);
			if (heredoc_result == 130)
				return ;
			else if (heredoc_result == -1)
				return ;
		}
		cmd = cmd->next;
	}
	if (is_debug_enabled(exec))
		debug_pipeline_commands(exec, cmd_list, var.cmd_count);
	if (var.cmd_count == 1)
	{
		execute_command_supreme(exec, cmd_list);
		return ;
	}
	var.pipe_count = var.cmd_count - 1;
	if (!init_pipes_and_env(exec, var.pipe_count, var.pipes, &env_array))
		return ;
	fork_and_exec_commands(exec, cmd_list, &var, env_array);
	close_all_pipes(var.pipe_count, var.pipes);
	exec->exit_status = wait_for_all_commands(exec, var.pids, var.cmd_count);
	if (is_debug_enabled(exec))
		ft_putstr_fd("minishell: pipeline execution complete\n", 2);
}




