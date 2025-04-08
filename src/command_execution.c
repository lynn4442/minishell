/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 09:46:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/04/07 23:47:56 by marvin           ###   ########.fr       */
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

void parse_and_execute(t_exec *exec, t_cmd_node *cmd)
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

	execute_command(cmd, &exec->gc);
}

void execute_command(t_cmd_node *cmd, t_gc *gc)
{
	int original_in = -1;
	int original_out = -1;
	pid_t pid;
	int status;
	char **env_array;

	if (setup_input_redirection(cmd, &original_in) == -1 ||
		setup_output_redirection(cmd, &original_out) == -1)
	{
		cmd->exec->exit_status = 1;
		return;
	}

	// Convert environment list to array for execve
	env_array = convert_env_to_array(cmd->exec, gc);
	if (!env_array)
	{
		perror("minishell: environment conversion failed");
		return;
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return;
	}
	if (pid == 0)
	{
		setup_child_signals();
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
		execve(cmd_path, cmd->arr, env_array);
		perror("minishell");
		ft_free_all(gc);
		exit(126);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGQUIT)
			{
				printf("\nQuit (core dumped)\n");
				cmd->exec->exit_status = 131;
			}
			else if (WTERMSIG(status) == SIGINT)
			{
				printf("\n");
				cmd->exec->exit_status = 130;
			}
		}
		else
			cmd->exec->exit_status = WEXITSTATUS(status);
			
		setup_interactive_signals();
	}
	restore_input_redirection(original_in);
	restore_output_redirection(original_out);
}
