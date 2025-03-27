/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 09:46:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/27 13:28:03 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *find_command_path(t_exec *exec, const char *cmd)
{
	char **path_dirs;
	char *full_path;
	char *temp_path;
	int i;

	if (!cmd)
		return (NULL);

	if (ft_strchr(cmd, '/') || ft_strncmp(cmd, "./", 2) == 0)
		return (ft_strdup(&exec->gc, cmd));

	path_dirs = get_path_from_env(exec);
	if (!path_dirs)
		return (NULL);

	i = 0;
	while (path_dirs[i])
	{
		temp_path = ft_strjoin(path_dirs[i], "/", &exec->gc);
		if (!temp_path)
		{
			i++;
			continue;
		}
		full_path = ft_strjoin(temp_path, cmd, &exec->gc);
		if (!full_path)
		{
			i++;
			continue;
		}
		if (access(full_path, X_OK) == 0)
			return (full_path);
		i++;
	}
	return (NULL);
}

static void handle_command_error(t_exec *exec, const char *cmd, int error_type)
{
	if (!cmd)
		return;

	if (error_type == 1)
	{
		printf("minihell: %s: command not found\n", cmd);
		exec->exit_status = 127;
	}
	else if (error_type == 2)
	{
		printf("minihell: %s: Permission denied\n", cmd);
		exec->exit_status = 1;
	}
	else if (error_type == 3)
	{
		printf("minihell: %s: No such file or directory\n", cmd);
		exec->exit_status = 1;
	}
}

void parse_and_execute(t_exec *exec, t_cmd_node *cmd, char **envp)
{
	if (!exec || !cmd || !cmd->arr || !cmd->arr[0])
	{
		printf("Error: Invalid command node\n");
		exec->exit_status = 1;
		return;
	}

	// For built-in commands
	if (is_builtin_command(cmd->arr[0]))
	{
		if (cmd->in || cmd->out)  // If there are redirections
		{
			pid_t pid = fork();
			if (pid == 0)
			{
				handle_redirection(cmd, &exec->gc);
				handle_builtin_command(exec, cmd);
				exit(exec->exit_status);
			}
			else
			{
				int status;
				waitpid(pid, &status, 0);
				if (WIFEXITED(status))
					exec->exit_status = WEXITSTATUS(status);
			}
		}
		else  // No redirections, execute builtin directly
		{
			handle_builtin_command(exec, cmd);
		}
		return;
	}

	// For external commands
	char *cmd_path = find_command_path(exec, cmd->arr[0]);
	if (!cmd_path)
	{
		handle_command_error(exec, cmd->arr[0], 1);
		return;
	}

	if (access(cmd_path, X_OK) != 0)
	{
		handle_command_error(exec, cmd->arr[0], 2);
		return;
	}

	execute_command(cmd, &exec->gc, envp);
}

void execute_command(t_cmd_node *cmd, t_gc *gc, char **envp)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return;
	}
	
	if (pid == 0)  // Child process
	{
		handle_redirection(cmd, gc);
		char *cmd_path = find_command_path(cmd->exec, cmd->arr[0]);
		if (!cmd_path)
		{
			ft_putstr_fd("minishell: command not found: ", 2);
			ft_putstr_fd(cmd->arr[0], 2);
			ft_putstr_fd("\n", 2);
			ft_free_all(gc);
			exit(127);
		}

		execve(cmd_path, cmd->arr, envp);
		perror("minishell");
		ft_free_all(gc);
		exit(126);
	}
	else  // Parent process
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			cmd->exec->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			cmd->exec->exit_status = 128 + WTERMSIG(status);
	}
}
