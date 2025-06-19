/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 09:18:08 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	execute_child_process(t_cmd_node *cmd, char *cmd_path, char **env_array)
{
	setup_child_signals();
	execve(cmd_path, cmd->arr, env_array);
	ft_putstr_fd("minishell: ", 2);
	perror(cmd->arr[0]);
	exit(126);
}

void	handle_child_exit_status(t_exec *exec, pid_t pid, t_cmd_node *cmd)
{
	int	status;
	int	sig;

	waitpid(pid, &status, 0);
	cleanup_heredoc_files(cmd);
	if (WIFEXITED(status))
		exec->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			exec->exit_status = 130;
		else if (sig == SIGQUIT)
		{
			printf("Quit (core dumped)\n");
			exec->exit_status = 131;
		}
		else if (sig == SIGKILL)
			exec->exit_status = 137;
		else if (sig == SIGSEGV)
			exec->exit_status = 139;
		else
			exec->exit_status = 128 + sig;
	}
}

int	prepare_process_environment(t_exec *exec, char ***env_array)
{
	*env_array = convert_env_to_array(exec, &exec->gc);
	if (!*env_array)
	{
		ft_putstr_fd("minishell: environment conversion failed\n", 2);
		exec->exit_status = 1;
		return (1);
	}
	return (0);
}

int	create_and_execute_process(t_exec *exec, t_cmd_node *cmd,
	char *cmd_path, char **env_array)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("minishell: fork error\n", 2);
		exec->exit_status = 1;
		return (1);
	}
	if (pid == 0)
		execute_child_process(cmd, cmd_path, env_array);
	else
		handle_child_exit_status(exec, pid, cmd);
	return (0);
}
