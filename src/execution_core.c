/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_core.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:24:00 by marvin            #+#    #+#             */
/*   Updated: 2025/05/17 17:24:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	launch_child_mission(t_cmd_node *cmd, char *cmd_path, char **env_array)
{
	setup_child_signals();
	execve(cmd_path, cmd->arr, env_array);
	ft_putstr_fd("minishell: ", 2);
	perror(cmd->arr[0]);
	exit(126);
}

void	wait_for_child_return(t_exec *exec, pid_t pid)
{
	int	status;
	
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exec->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exec->exit_status = 128 + WTERMSIG(status);
}

int	run_builtin_cmd(t_exec *exec, t_cmd_node *cmd)
{
	return (handle_builtin_command(exec, cmd));
}

int	execute_external_quest(t_exec *exec, t_cmd_node *cmd)
{
	char	*cmd_path;
	char	**env_array;
	pid_t	pid;
	char	*expanded_cmd;

	if (prep_cmd_for_launch(exec, cmd, &expanded_cmd, &cmd_path))
		return (1);
	env_array = convert_env_to_array(exec, &exec->gc);
	if (!env_array)
	{
		ft_putstr_fd("minishell: environment conversion failed\n", 2);
		exec->exit_status = 1;
		return (1);
	}
	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("minishell: fork error\n", 2);
		exec->exit_status = 1;
		return (1);
	}
	if (pid == 0)
		launch_child_mission(cmd, cmd_path, env_array);
	else
		wait_for_child_return(exec, pid);
	return (0);
}

void	execute_command_supreme(t_exec *exec, t_cmd_node *cmd)
{
	int	original_in;
	int	original_out;
	int	is_builtin;

	original_in = -1;
	original_out = -1;
	if (!exec || !cmd || !cmd->arr || !cmd->arr[0])
		return ;
	is_builtin = is_builtin_command(cmd->arr[0]);
	if (cmd->type == PIPE)
	{
		execute_with_pipes(exec, exec->cmd_list);
		return ;
	}
	if (is_builtin && !cmd->in && !cmd->out)
	{
		run_builtin_cmd(exec, cmd);
		return ;
	}
	if (setup_redirections(cmd, &original_in, &original_out) == -1)
		return ;
	if (is_builtin)
		run_builtin_cmd(exec, cmd);
	else
		execute_external_quest(exec, cmd);
	restore_og_redirections(original_in, original_out);
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