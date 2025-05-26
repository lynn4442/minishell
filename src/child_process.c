/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:30:00 by marvin            #+#    #+#             */
/*   Updated: 2025/05/26 22:38:48 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_child_process(t_cmd_node *cmd, char *cmd_path, char **env_array)
{
	setup_child_signals();
	execve(cmd_path, cmd->arr, env_array);
	ft_putstr_fd("minishell: ", 2);
	perror(cmd->arr[0]);
	exit(126);
}

//0	Success (No error)
//1	General error (Miscellaneous error)
//2	Incorrect usage (Misuse of shell builtins)
//126	Command found but not executable
//127	Command not found
//128	Invalid argument to exit
//130	Command terminated by Ctrl+C (SIGINT)
//137	Command killed with kill -9 (SIGKILL)
//139	Segmentation fault (SIGSEGV)
//255	Exit status out of range 

void	handle_child_exit_status(t_exec *exec, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exec->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		int sig = WTERMSIG(status);
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
	{
		function_ignore();  // Child process ignores signals
		execute_child_process(cmd, cmd_path, env_array);
	}
	else
	{
		function_ignore();  // Parent process ignores signals while waiting
		handle_child_exit_status(exec, pid);
	}
	return (0);
}
