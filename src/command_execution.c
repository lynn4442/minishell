/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 09:46:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/05/13 13:36:45 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//find the path of a command
char *find_command_path(t_exec *exec, const char *cmd)
{
	char **path_dirs;
	char *full_path;
	char *temp_path;
	int i;
	char *expanded_cmd;

	if (!cmd)
		return (NULL);

	//expand env var in command name
	expanded_cmd = process_quotes(cmd, exec->env_list, exec);
	if (!expanded_cmd)
		return (NULL);
	// check if the input is something like this /bin/ls (doesnt need to look in path)
	if (ft_strchr(expanded_cmd, '/') || ft_strncmp(expanded_cmd, "./", 2) == 0)
		return (ft_strdup(&exec->gc, expanded_cmd));
	// not relative or absolute path we need to check in the PATH
	path_dirs = get_path_from_env(exec);
	if (!path_dirs) // in case PATH not set || empty --> only absolute com can run
	{
		full_path = ft_strjoin("./", expanded_cmd, &exec->gc);
		if (full_path && access(full_path, F_OK) == 0) // F_OK to make sure that the file exist
			return full_path;
		return (NULL);
	}
	i = 0;
	while (path_dirs[i]) // go over each part in the PATH
	{
		temp_path = ft_strjoin(path_dirs[i], "/", &exec->gc);
		if (!temp_path)
		{
			i++;
			continue;
		}
		full_path = ft_strjoin(temp_path, expanded_cmd, &exec->gc);
		if (!full_path)
		{
			i++;
			continue;
		}
		if (access(full_path, F_OK) == 0)
		{
			if (access(full_path, X_OK) == 0) // X_OK --> checks if file is executable
				return (full_path);
			else
				perror("minishell");
			break;
			// stops when the file is found but not executable 
		}
		i++;
	}
	return (NULL);
}

// error handling
static void handle_command_error(t_exec *exec, const char *cmd, int error_type)
{
	if (!cmd)
		return;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	if (error_type == 1)
	{
		ft_putstr_fd(": command not found\n", 2);
		exec->exit_status = 127;
	}
	else if (error_type == 2)
	{
		ft_putstr_fd(": Permission denied\n", 2);
		exec->exit_status = 1;
	}
	else if (error_type == 3)
	{
		ft_putstr_fd(": No such file or directory\n", 2);
		exec->exit_status = 1;
	}
	else
	{
		ft_putstr_fd(": Error executing command\n", 2);
		exec->exit_status = 1;
	}
}

// parsing and executing
void parse_and_execute(t_exec *exec, t_cmd_node *cmd)
{
	if (!exec || !cmd || !cmd->arr || !cmd->arr[0])
	{
		ft_putstr_fd("minishell: Error: Invalid command node\n", 2);
		exec->exit_status = 1;
		return;
	}

	// Use the generalized execution system for all commands
	execute_command_generic(exec, cmd);
}

/* Restore standard I/O */
static void restore_redirections(int original_in, int original_out)
{
	if (original_in != -1)
	{
		dup2(original_in, STDIN_FILENO);
		close(original_in);
	}
	
	if (original_out != -1)
	{
		dup2(original_out, STDOUT_FILENO);
		close(original_out);
	}
}

/* Setup standard I/O redirections */
static int setup_redirections(t_cmd_node *cmd, int *original_in, int *original_out)
{
	// Save original file descriptors
	*original_in = dup(STDIN_FILENO);
	*original_out = dup(STDOUT_FILENO);
	
	// Handle input redirection
	if (cmd->in)
	{
		int fd = open(cmd->in, O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->in, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			cmd->exec->exit_status = 1;
			restore_redirections(*original_in, *original_out);
			return (-1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	
	// Handle output redirection
	if (cmd->out)
	{
		int flags = O_WRONLY | O_CREAT;
		flags |= cmd->append ? O_APPEND : O_TRUNC;
		
		int fd = open(cmd->out, flags, 0644);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->out, 2);
			ft_putstr_fd(": Error opening output file\n", 2);
			cmd->exec->exit_status = 1;
			restore_redirections(*original_in, *original_out);
			return (-1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	
	return (0);
}

/* Execute a builtin command */
static int execute_builtin(t_exec *exec, t_cmd_node *cmd)
{
	return (handle_builtin_command(exec, cmd));
}

/* Execute an external command */
static int execute_external(t_exec *exec, t_cmd_node *cmd)
{
	char *cmd_path;
	char **env_array;
	pid_t pid;
	int status;
	char *expanded_cmd;

	// Expand environment variables in command name
	expanded_cmd = process_quotes(cmd->arr[0], exec->env_list, exec);
	if (!expanded_cmd)
	{
		handle_command_error(exec, cmd->arr[0], 1);
		return (1);
	}

	// Find command path
	cmd_path = find_command_path(exec, expanded_cmd);
	if (!cmd_path)
	{
		// Path not found, check if it's a direct executable
		if (ft_strchr(expanded_cmd, '/'))
		{
			// It has a path but doesn't exist or can't be accessed
			if (access(expanded_cmd, F_OK) != 0)
				handle_command_error(exec, expanded_cmd, 3); // No such file or directory
			else if (access(expanded_cmd, X_OK) != 0)
				handle_command_error(exec, expanded_cmd, 2); // Permission denied
			else
				handle_command_error(exec, expanded_cmd, 0); // Generic error
		}
		else
		{
			// Simple command that wasn't found in PATH
			handle_command_error(exec, expanded_cmd, 1); // Command not found
		}
		return (1);
	}

	// Check command permissions - already checked in find_command_path
	if (access(cmd_path, X_OK) != 0)
	{
		handle_command_error(exec, expanded_cmd, 2);
		return (1);
	}

	// Convert environment list to array for execve
	env_array = convert_env_to_array(exec, &exec->gc);
	if (!env_array)
	{
		ft_putstr_fd("minishell: environment conversion failed\n", 2);
		exec->exit_status = 1;
		return (1);
	}

	// Fork and execute command
	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("minishell: fork error\n", 2);
		exec->exit_status = 1;
		return (1);
	}
	
	if (pid == 0)
	{
		// Child process
		setup_child_signals();
		
		execve(cmd_path, cmd->arr, env_array);
		
		// If execve fails
		ft_putstr_fd("minishell: ", 2);
		perror(expanded_cmd);
		exit(126);
	}
	else
	{
		// Parent process
		waitpid(pid, &status, 0);
		
		// Update exit status
		if (WIFEXITED(status))
			exec->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			exec->exit_status = 128 + WTERMSIG(status);
	}
	
	return (0);
}

/* Generalized command execution that handles all command types */
void execute_command_generic(t_exec *exec, t_cmd_node *cmd)
{
	int original_in = -1;
	int original_out = -1;
	int is_builtin;

	if (!exec || !cmd || !cmd->arr || !cmd->arr[0])
		return;

	// Check if command is a builtin
	is_builtin = is_builtin_command(cmd->arr[0]);

	// Handle piped commands separately
	if (cmd->type == PIPE)
	{
		execute_with_pipes(exec, exec->cmd_list);
		return;
	}

	// Handle builtins without redirection directly for efficiency
	if (is_builtin && !cmd->in && !cmd->out)
	{
		execute_builtin(exec, cmd);
		return;
	}
	
	// For builtins with redirections, or external commands
	// Setup redirections
	if (setup_redirections(cmd, &original_in, &original_out) == -1)
		return;
	
	// Execute command based on type
	if (is_builtin)
	{
		execute_builtin(exec, cmd);
	}
	else
	{
		execute_external(exec, cmd);
	}
	
	// Restore original redirections
	restore_redirections(original_in, original_out);
}

/* Execute a command with the generalized executor */
void execute_command(t_cmd_node *cmd)
{
	// This function now wraps the more general executor
	if (!cmd || !cmd->exec)
		return;
	execute_command_generic(cmd->exec, cmd);
}
