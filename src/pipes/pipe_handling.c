/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/25 23:19:20 by lyoussef         ###   ########.fr       */
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

static int	process_all_heredocs(t_exec *exec, t_cmd_node *cmd_list)
{
	t_cmd_node	*cmd;
	int			heredoc_result;

	cmd = cmd_list;
	while (cmd)
	{
		if (cmd->heredoc_delimiter)
		{
			heredoc_result = handle_heredoc(cmd, exec);
			if (heredoc_result == 130)
				return (130);
			else if (heredoc_result == -1)
				return (-1);
		}
		cmd = cmd->next;
	}
	return (0);
}

static void	handle_single_command(t_exec *exec, t_cmd_node *cmd_list)
{
	execute_command_supreme(exec, cmd_list);
}

static int	setup_pipeline_execution(t_exec *exec, t_r_variables *var,
	char ***env_array)
{
	var->pipe_count = var->cmd_count - 1;
	if (!init_pipes_and_env(exec, var->pipe_count, var->pipes, env_array))
		return (0);
	return (1);
}

static void	cleanup_and_wait(t_exec *exec, t_r_variables *var)
{
	close_all_pipes(var->pipe_count, var->pipes);
	exec->exit_status = wait_for_all_commands(exec, var->pids, var->cmd_count);
	if (is_debug_enabled(exec))
		ft_putstr_fd("minishell: pipeline execution complete\n", 2);
}

void	execute_with_pipes(t_exec *exec, t_cmd_node *cmd_list)
{
	t_r_variables	var;
	char			**env_array;
	int				heredoc_result;

	var.cmd_count = count_pipeline_commands(cmd_list);
	if (var.cmd_count <= 0)
		return ;
	heredoc_result = process_all_heredocs(exec, cmd_list);
	if (heredoc_result != 0)
		return ;
	if (is_debug_enabled(exec))
		debug_pipeline_commands(exec, cmd_list, var.cmd_count);
	if (var.cmd_count == 1)
	{
		handle_single_command(exec, cmd_list);
		return ;
	}
	if (!setup_pipeline_execution(exec, &var, &env_array))
		return ;
	fork_and_exec_commands(exec, cmd_list, &var, env_array);
	cleanup_and_wait(exec, &var);
}
