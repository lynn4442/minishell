/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:48:14 by hhussein          #+#    #+#             */
/*   Updated: 2025/06/21 23:18:07 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipes.h"

static void	execute_pipeline_command(t_cmd_node *cmd, char **env_array)
{
	int		debug;
	char	*cmd_path;

	debug = is_debug_enabled(cmd->exec);
	if (debug)
	{
		print_command_debug_info(cmd);
		if (cmd->in || cmd->out)
			print_redirection_debug_info(cmd);
	}
	handle_builtin_or_empty(cmd);
	cmd_path = find_command_path(cmd->exec, cmd->arr[0]);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->arr[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	execve(cmd_path, cmd->arr, env_array);
	ft_putstr_fd("minishell: ", 2);
	perror(cmd->arr[0]);
	exit(126);
}

static	void	fork_exec_helper(int debug, t_r_variables *var,
				t_cmd_node *current, char **env_array)
{
	setup_child_signals();
	if (debug)
		print_child_debug(current);
	setup_child_pipes(current, var->i, var->pipe_count, var->pipes);
	close_all_pipes(var->pipe_count, var->pipes);
	execute_pipeline_command(current, env_array);
	exit(1);
}

void	fork_and_exec_commands(t_exec *exec, t_cmd_node *cmd_list,
	t_r_variables *var, char **env_array)
{
	t_cmd_node	*current;
	int			debug;

	current = cmd_list;
	debug = is_debug_enabled(exec);
	var->i = -1;
	while (++var->i < var->cmd_count)
	{
		if (debug)
			print_parent_debug(exec, var->i, var->cmd_count);
		var->pids[var->i] = fork();
		if (var->pids[var->i] == -1)
		{
			perror("fork");
			close_all_pipes(var->pipe_count, var->pipes);
			return ;
		}
		if (var->pids[var->i] == 0)
			fork_exec_helper(debug, var, current, env_array);
		current = current->next;
	}
}

static int	compute_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (128 + WTERMSIG(status));
}

int	wait_for_all_commands(t_exec *exec, pid_t *pids, int cmd_count)
{
	int	status;
	int	last_status;
	int	debug;
	int	i;

	i = -1;
	debug = is_debug_enabled(exec);
	last_status = 0;
	while (++i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (debug)
			print_wait_debug(exec, i, status);
		if (i == cmd_count - 1)
			last_status = compute_exit_status(status);
	}
	return (last_status);
}
