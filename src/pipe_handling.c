/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/05/22 16:17:10 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Check if debug mode is enabled via environment variable */
static int is_debug_enabled(t_exec *exec)
{
	t_env_var *debug_var = get_env_var(exec, "MINISHELL_DEBUG");
	return (debug_var && debug_var->value &&
			(ft_strcmp(debug_var->value, "1") == 0 ||
			 ft_strcmp(debug_var->value, "true") == 0));
}

/* Setup a single command's input from a pipe or file */
// static void setup_pipe_input(t_cmd_node *cmd, int prev_pipe_fd)
// {
// 	int	fd;

// 	if (cmd->in)// Handle input redirection first
// 	{
// 		// Input redirection
// 		fd = open(cmd->in, O_RDONLY);
// 		if (fd == -1)
// 		{
// 			ft_putstr_fd("minishell: ", 2);
// 			ft_putstr_fd(cmd->in, 2);
// 			ft_putstr_fd(": No such file or directory\n", 2);
// 			cmd->exec->exit_status = 1;// Don't exit, just set error status and continue
// 			if (prev_pipe_fd != -1)// Close pipe read end if it exists
// 				close(prev_pipe_fd);
// 			return ;
// 		}
// 		if (dup2(fd, STDIN_FILENO) == -1)// Redirect stdin from the file
// 			perror("dup2 input file");
// 		close(fd);
// 		if (prev_pipe_fd != -1)// We're done with input - close pipe read end if it exists
// 			close(prev_pipe_fd);
// 		return ;
// 	}
// 	if (prev_pipe_fd != -1)	// If no input redirection, use pipe if available
// 	{
// 		if (dup2(prev_pipe_fd, STDIN_FILENO) == -1)
// 			perror("dup2 pipe input");
// 		close(prev_pipe_fd);
// 	}
// }
static void	print_input_error(t_cmd_node *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->in, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	cmd->exec->exit_status = 1;
}
static void	handle_input_file(t_cmd_node *cmd, int fd, int prev_pipe_fd)
{
	if (dup2(fd, STDIN_FILENO) == -1)
		perror("dup2 input file");
	close(fd);
	if (prev_pipe_fd != -1)
		close(prev_pipe_fd);
}
static void	handle_pipe_input(int prev_pipe_fd)
{
	if (dup2(prev_pipe_fd, STDIN_FILENO) == -1)
		perror("dup2 pipe input");
	close(prev_pipe_fd);
}
static void	setup_pipe_input(t_cmd_node *cmd, int prev_pipe_fd)
{
	int	fd;

	if (cmd->in)
	{
		fd = open(cmd->in, O_RDONLY);
		if (fd == -1)
		{
			print_input_error(cmd);
			if (prev_pipe_fd != -1)
				close(prev_pipe_fd);
			return ;
		}
		handle_input_file(cmd, fd, prev_pipe_fd);
		return ;
	}
	if (prev_pipe_fd != -1)
		handle_pipe_input(prev_pipe_fd);
}
/* Setup a single command's output to a pipe or file */
// static void setup_pipe_output(t_cmd_node *cmd, int next_pipe_fd)
// {
// 	if (cmd->out)// Handle output redirection first
// 	{
// 		int flags = O_WRONLY | O_CREAT;
// 		flags |= cmd->append ? O_APPEND : O_TRUNC;
// 		int file_fd = open(cmd->out, flags, 0644);// Open the output file
// 		if (file_fd == -1)
// 		{
// 			ft_putstr_fd("minishell: ", 2);
// 			ft_putstr_fd(cmd->out, 2);
// 			ft_putstr_fd(": Error opening output file\n", 2);
// 			exit(1);
// 		}
// 		if (next_pipe_fd != -1)// If we also have a pipe, we need to duplicate output to both
// 		{
// 			int tee_pipe[2];// Create a temporary pipe for tee-like functionality
// 			if (pipe(tee_pipe) == -1)
// 			{
// 				perror("pipe");
// 				close(file_fd);
// 				exit(1);
// 			}
// 			pid_t tee_pid = fork();// Fork a child to handle the tee operation
// 			if (tee_pid == -1)
// 			{
// 				perror("fork");
// 				close(file_fd);
// 				close(tee_pipe[0]);
// 				close(tee_pipe[1]);
// 				exit(1);
// 			}
// 			if (tee_pid == 0)
// 			{
// 				char buffer[4096];// Child process: read from pipe and write to both outputs
// 				ssize_t n;
// 				close(tee_pipe[1]);  // Close write end
// 				while ((n = read(tee_pipe[0], buffer, sizeof(buffer))) > 0)
// 				{
// 					if (write(file_fd, buffer, n) != n)// Write to file
// 					{
// 						perror("write to file");
// 						exit(1);
// 					}
// 					if (write(next_pipe_fd, buffer, n) != n)// Write to next pipe
// 					{
// 						perror("write to pipe");
// 						exit(1);
// 					}
// 				}
// 				close(tee_pipe[0]);
// 				close(file_fd);
// 				close(next_pipe_fd);
// 				exit(0);
// 			}
// 			else
// 			{
// 				close(tee_pipe[0]);  // Close read end// Parent process: redirect stdout to the tee pipe
// 				if (dup2(tee_pipe[1], STDOUT_FILENO) == -1)
// 				{
// 					perror("dup2");
// 					exit(1);
// 				}
// 				close(tee_pipe[1]);
// 				close(file_fd);// Don't close next_pipe_fd as the tee process needs it
// 			}
// 		}
// 		else
// 		{
// 			if (dup2(file_fd, STDOUT_FILENO) == -1)// No pipe, just redirect to file
// 			{
// 				perror("dup2");
// 				exit(1);
// 			}
// 			close(file_fd);
// 		}
// 	}
// 	else if (next_pipe_fd != -1)
// 	{
// 		if (dup2(next_pipe_fd, STDOUT_FILENO) == -1)// No file redirection, just pipe
// 		{
// 			perror("dup2");
// 			exit(1);
// 		}
// 		close(next_pipe_fd);
// 	}
// }
static void print_output_error(t_cmd_node *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->out, 2);
	ft_putstr_fd(": Error opening output file\n", 2);
	exit(1);
}

static void handle_tee_child(int file_fd, int next_pipe_fd, int read_end)
{
	char buffer[4096];
	ssize_t n;

	close(read_end + 1);
	while ((n = read(read_end, buffer, sizeof(buffer))) > 0)
	{
		if (write(file_fd, buffer, n) != n)
			perror("write to file"), exit(1);
		if (write(next_pipe_fd, buffer, n) != n)
			perror("write to pipe"), exit(1);
	}
	close(read_end);
	close(file_fd);
	close(next_pipe_fd);
	exit(0);
}

static void setup_tee_pipe(int file_fd, int next_pipe_fd)
{
	int tee_pipe[2];
	pid_t tee_pid;

	if (pipe(tee_pipe) == -1)
		perror("pipe"), close(file_fd), exit(1);
	tee_pid = fork();
	if (tee_pid == -1)
		perror("fork"), close(file_fd), close(tee_pipe[0]), close(tee_pipe[1]), exit(1);
	if (tee_pid == 0)
		handle_tee_child(file_fd, next_pipe_fd, tee_pipe[0]);
	close(tee_pipe[0]);
	if (dup2(tee_pipe[1], STDOUT_FILENO) == -1)
		perror("dup2"), exit(1);
	close(tee_pipe[1]);
	close(file_fd);
}

static void redirect_to_file(int file_fd)
{
	if (dup2(file_fd, STDOUT_FILENO) == -1)
		perror("dup2"), exit(1);
	close(file_fd);
}

static void redirect_to_pipe(int next_pipe_fd)
{
	if (dup2(next_pipe_fd, STDOUT_FILENO) == -1)
		perror("dup2"), exit(1);
	close(next_pipe_fd);
}

static void setup_pipe_output(t_cmd_node *cmd, int next_pipe_fd)
{
	int flags;
	int file_fd;

	if (cmd->out)
	{
		flags = O_WRONLY | O_CREAT;
		if (cmd->append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		file_fd = open(cmd->out, flags, 0644);
		if (file_fd == -1)
			print_output_error(cmd);
		if (next_pipe_fd != -1)
			setup_tee_pipe(file_fd, next_pipe_fd);
		else
			redirect_to_file(file_fd);
	}
	else if (next_pipe_fd != -1)
		redirect_to_pipe(next_pipe_fd);
}
/* Execute a command in a pipeline (child process code) */
// static void execute_pipeline_command(t_cmd_node *cmd, char **env_array)
// {
// 	int debug = is_debug_enabled(cmd->exec);
	
// 	if (debug)// Debug info to stderr (won't interfere with redirections)
// 	{
// 		ft_putstr_fd("minishell: executing command: ", 2);
// 		for (int i = 0; cmd->arr && cmd->arr[i]; i++)
// 		{
// 			ft_putstr_fd(cmd->arr[i], 2);
// 			ft_putstr_fd(" ", 2);
// 		}
// 		ft_putstr_fd("\n", 2);
// 		if (cmd->in)
// 		{
// 			ft_putstr_fd("minishell: with input redirection from: ", 2);
// 			ft_putstr_fd(cmd->in, 2);
// 			ft_putstr_fd("\n", 2);
// 		}
// 		if (cmd->out)
// 		{
// 			ft_putstr_fd("minishell: with output redirection to: ", 2);
// 			ft_putstr_fd(cmd->out, 2);
// 			ft_putstr_fd(cmd->append ? " (append)" : " (truncate)", 2);
// 			ft_putstr_fd("\n", 2);
// 		}
// 	}
	
// 	if (!cmd->arr || !cmd->arr[0])// Verify command exists
// 		exit(1);// ft_putstr_fd("minishell: empty command\n", 2);
// 	if (is_builtin_command(cmd->arr[0]))// Handle builtin commands
// 	{
// 		handle_builtin_command(cmd->exec, cmd);
// 		exit(cmd->exec->exit_status);
// 	}
// 	char *cmd_path = find_command_path(cmd->exec, cmd->arr[0]);// Handle external commands
// 	if (!cmd_path)
// 	{
// 		ft_putstr_fd("minishell: ", 2);
// 		ft_putstr_fd(cmd->arr[0], 2);
// 		ft_putstr_fd(": command not found\n", 2);
// 		exit(127);
// 	}
// 	execve(cmd_path, cmd->arr, env_array);
// 	ft_putstr_fd("minishell: ", 2);	// If execve fails
// 	perror(cmd->arr[0]);
// 	exit(126);
// }
static void	print_command_debug_info(t_cmd_node *cmd)
{
	int	i;

	ft_putstr_fd("minishell: executing command: ", 2);
	i = 0;
	while (cmd->arr && cmd->arr[i])
	{
		ft_putstr_fd(cmd->arr[i], 2);
		ft_putstr_fd(" ", 2);
		i++;
	}
	ft_putstr_fd("\n", 2);
}
static void	print_redirection_debug_info(t_cmd_node *cmd)
{
	ft_putstr_fd("minishell: with input redirection from: ", 2);
	ft_putstr_fd(cmd->in, 2);
	ft_putstr_fd("\n", 2);
	ft_putstr_fd("minishell: with output redirection to: ", 2);
	ft_putstr_fd(cmd->out, 2);
	if (cmd->append)
		ft_putstr_fd(" (append)", 2);
	else
		ft_putstr_fd(" (truncate)", 2);
	ft_putstr_fd("\n", 2);
}
static void	handle_execve_failure(char *name)
{
	ft_putstr_fd("minishell: ", 2);
	perror(name);
	exit(126);
}
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
	handle_execve_failure(cmd->arr[0]);
}

/* Close all unused pipe file descriptors */
static void close_all_pipes(int pipe_count, int pipes[][2])
{
	int	i;

	i = 0;
	while ( i < pipe_count)
	{
		if (pipes[i][0] >= 0)  // Only close valid descriptors
			close(pipes[i][0]);
		if (pipes[i][1] >= 0)  // Only close valid descriptors
			close(pipes[i][1]);
		i++;
	}
}

/* Main function to execute a pipeline of commands */
// void execute_with_pipes(t_exec *exec, t_cmd_node *cmd_list)
// {
// 	t_cmd_node *current = cmd_list;
// 	int cmd_count = 0;
// 	int pipe_count = 0;
// 	int debug = is_debug_enabled(exec);
// 	// Count commands in pipeline
// 	while (current)
// 	{
// 		cmd_count++;
// 		current = current->next;
// 	}
// 	if (cmd_count <= 0)
// 		return;
// 	if (debug)// Debug output - show the command pipeline
// 	{
// 		ft_putstr_fd("minishell: setting up pipeline with ", 2);
// 		ft_putstr_fd(ft_itoa(cmd_count, &exec->gc), 2);
// 		ft_putstr_fd(" commands\n", 2);
// 		current = cmd_list;// Print command details
// 		int i = 0;
// 		while (current)
// 		{
// 			ft_putstr_fd("minishell: cmd", 2);
// 			ft_putstr_fd(ft_itoa(i++, &exec->gc), 2);
// 			ft_putstr_fd(": ", 2);
// 			if (current->arr && current->arr[0])
// 			{
// 				for (int j = 0; current->arr[j]; j++)
// 				{
// 					ft_putstr_fd(current->arr[j], 2);
// 					ft_putstr_fd(" ", 2);
// 				}
// 			}
// 			if (current->out)
// 			{
// 				if (current->append)
// 					ft_putstr_fd(">> ", 2);
// 				else
// 					ft_putstr_fd("> ", 2);
// 				ft_putstr_fd(current->out, 2);
// 				ft_putstr_fd(" ", 2);
// 			}
// 			if (current->in)
// 			{
// 				ft_putstr_fd("< ", 2);
// 				ft_putstr_fd(current->in, 2);
// 				ft_putstr_fd(" ", 2);
// 			}

// 			ft_putstr_fd("\n", 2);
// 			current = current->next;
// 		}
// 	}
// 	if (cmd_count == 1)// If only one command, use the standard execution
// 	{
// 		execute_command_supreme(exec, cmd_list);
// 		return ;
// 	}
// 	pipe_count = cmd_count - 1;// For multiple commands, we need pipes
// 	int pipes[pipe_count][2];
// 	pid_t pids[cmd_count];
// 	for (int i = 0; i < pipe_count; i++)// Initialize pipe descriptors to -1
// 	{
// 		pipes[i][0] = -1;
// 		pipes[i][1] = -1;
// 	}
// 	for (int i = 0; i < pipe_count; i++)// Create all pipes
// 	{
// 		if (pipe(pipes[i]) == -1)
// 		{
// 			perror("pipe");
// 			close_all_pipes(i, pipes); // Close any pipes created so far
// 			return;
// 		}
// 	}
// 	char **env_array = convert_env_to_array(exec, &exec->gc);// Convert environment to array for child processes
// 	if (!env_array)
// 	{
// 		ft_putstr_fd("minishell: environment conversion failed\n", 2);
// 		close_all_pipes(pipe_count, pipes);
// 		return;
// 	}
// 	current = cmd_list;// Fork for each command
// 	for (int i = 0; i < cmd_count; i++)
// 	{
// 		if (debug)// Debug output - Command being executed
// 		{
// 			ft_putstr_fd("minishell: setting up command ", 2);
// 			ft_putstr_fd(ft_itoa(i+1, &exec->gc), 2);
// 			ft_putstr_fd("/", 2);
// 			ft_putstr_fd(ft_itoa(cmd_count, &exec->gc), 2);
// 			ft_putstr_fd("\n", 2);
// 		}
// 		pids[i] = fork();
// 		if (pids[i] == -1)
// 		{
// 			perror("fork");
// 			close_all_pipes(pipe_count, pipes);
// 			return;
// 		}
// 		if (pids[i] == 0)
// 		{
// 			setup_child_signals();// Child process
// 			if (debug)// Debug - show command details
// 			{
// 				ft_putstr_fd("minishell: child process for command: ", 2);
// 				if (current->arr && current->arr[0])
// 					ft_putstr_fd(current->arr[0], 2);
// 				ft_putstr_fd("\n", 2);
// 			}
// 			int prev_pipe_fd = (i > 0) ? pipes[i - 1][0] : -1;// Setup input (from previous pipe or file)
// 			setup_pipe_input(current, prev_pipe_fd);
// 			int next_pipe_fd = (i < pipe_count) ? pipes[i][1] : -1;// Setup output (to next pipe or file)
// 			setup_pipe_output(current, next_pipe_fd);
// 			close_all_pipes(pipe_count, pipes);// Close all other pipe file descriptors
// 			execute_pipeline_command(current, env_array);// Execute the command
// 			exit(1);// Should never reach here
// 		}
// 		current = current->next;// Parent process continues
// 	}
// 	close_all_pipes(pipe_count, pipes);// Parent: close all pipe file descriptors
// 	int status;// Wait for all children to finish
// 	int last_status = 0;
// 	for (int i = 0; i < cmd_count; i++)
// 	{
// 		waitpid(pids[i], &status, 0);
// 		if (debug)// Debug - show exit status
// 		{
// 			ft_putstr_fd("minishell: command ", 2);
// 			ft_putstr_fd(ft_itoa(i+1, &exec->gc), 2);
// 			ft_putstr_fd(" finished with status ", 2);
// 			ft_putstr_fd(ft_itoa(WEXITSTATUS(status), &exec->gc), 2);
// 			ft_putstr_fd("\n", 2);
// 		}
// 		if (i == cmd_count - 1)// Save status of the last command in the pipeline
// 		{
// 			if (WIFEXITED(status))
// 				last_status = WEXITSTATUS(status);
// 			else if (WIFSIGNALED(status))
// 				last_status = 128 + WTERMSIG(status);
// 		}
// 	}
// 	exec->exit_status = last_status;// Set the exit status from the last command in the pipeline
// 	if (debug)// Debug - pipeline execution complete
// 	{
// 		ft_putstr_fd("minishell: pipeline execution complete\n", 2);
// 	}
// }

static int	count_pipeline_commands(t_cmd_node *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

static void	debug_print_cmd(t_exec *exec, t_cmd_node *cmd, int index)
{
	int	j;

	ft_putstr_fd("minishell: cmd", 2);
	ft_putstr_fd(ft_itoa(index, &exec->gc), 2);
	ft_putstr_fd(": ", 2);
	j = 0;
	while (cmd->arr && cmd->arr[j])
	{
		ft_putstr_fd(cmd->arr[j], 2);
		ft_putstr_fd(" ", 2);
		j++;
	}
	if (cmd->out)
	{
		ft_putstr_fd(cmd->append ? ">> " : "> ", 2);
		ft_putstr_fd(cmd->out, 2);
		ft_putstr_fd(" ", 2);
	}
	if (cmd->in)
	{
		ft_putstr_fd("< ", 2);
		ft_putstr_fd(cmd->in, 2);
		ft_putstr_fd(" ", 2);
	}
	ft_putstr_fd("\n", 2);
}

static void	debug_pipeline_commands(t_exec *exec,
	t_cmd_node *cmd_list, int cmd_count)
{
	t_cmd_node	*current;
	int			index;

	ft_putstr_fd("minishell: setting up pipeline with ", 2);
	ft_putstr_fd(ft_itoa(cmd_count, &exec->gc), 2);
	ft_putstr_fd(" commands\n", 2);
	current = cmd_list;
	index = 0;
	while (current)
	{
		debug_print_cmd(exec, current, index);
		current = current->next;
		index++;
	}
}

static int	create_all_pipes(int pipe_count, int pipes[][2])
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe");
			close_all_pipes(i, pipes);
			return (0);
		}
		i++;
	}
	return (1);
}

static void	print_child_debug(t_cmd_node *current)
{
	if (current->arr && current->arr[0])
	{
		ft_putstr_fd("minishell: child process for command: ", 2);
		ft_putstr_fd(current->arr[0], 2);
		ft_putstr_fd("\n", 2);
	}
}

static void	print_parent_debug(t_exec *exec, int i, int cmd_count)
{
	ft_putstr_fd("minishell: setting up command ", 2);
	ft_putstr_fd(ft_itoa(i + 1, &exec->gc), 2);
	ft_putstr_fd("/", 2);
	ft_putstr_fd(ft_itoa(cmd_count, &exec->gc), 2);
	ft_putstr_fd("\n", 2);
}

static void	setup_child_pipes(t_cmd_node *current, int i,
	int pipe_count, int pipes[][2])
{
	if (i > 0)
		setup_pipe_input(current, pipes[i - 1][0]);
	else
		setup_pipe_input(current, -1);
	if (i < pipe_count)
		setup_pipe_output(current, pipes[i][1]);
	else
		setup_pipe_output(current, -1);
}

static void	fork_and_exec_commands(t_exec *exec, t_cmd_node *cmd_list,
	int pipes[][2], int cmd_count, int pipe_count,
	char **env_array, pid_t *pids)
{
	t_cmd_node	*current;
	int			i;
	int			debug;

	current = cmd_list;
	debug = is_debug_enabled(exec);
	i = -1;
	while (++i < cmd_count)
	{
		if (debug)
			print_parent_debug(exec, i, cmd_count);
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork");
			close_all_pipes(pipe_count, pipes);
			return ;
		}
		if (pids[i] == 0)
		{
			setup_child_signals();
			if (debug)
				print_child_debug(current);
			setup_child_pipes(current, i, pipe_count, pipes);
			close_all_pipes(pipe_count, pipes);
			execute_pipeline_command(current, env_array);
			exit(1);
		}
		current = current->next;
	}
}

static void	print_wait_debug(t_exec *exec, int i, int status)
{
	ft_putstr_fd("minishell: command ", 2);
	ft_putstr_fd(ft_itoa(i + 1, &exec->gc), 2);
	ft_putstr_fd(" finished with status ", 2);
	ft_putstr_fd(ft_itoa(WEXITSTATUS(status), &exec->gc), 2);
	ft_putstr_fd("\n", 2);
}

static int	compute_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (128 + WTERMSIG(status));
}

static int	wait_for_all_commands(t_exec *exec, pid_t *pids, int cmd_count)
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
	int		cmd_count;
	int		pipe_count;
	int		pipes[1024][2];
	pid_t	pids[1024];
	char	**env_array;

	cmd_count = count_pipeline_commands(cmd_list);
	if (cmd_count <= 0)
		return ;
	if (is_debug_enabled(exec))
		debug_pipeline_commands(exec, cmd_list, cmd_count);
	if (cmd_count == 1)
	{
		execute_command_supreme(exec, cmd_list);
		return ;
	}
	pipe_count = cmd_count - 1;
	if (!init_pipes_and_env(exec, pipe_count, pipes, &env_array))
		return ;
	fork_and_exec_commands(exec, cmd_list, pipes, cmd_count,
		pipe_count, env_array, pids);
	close_all_pipes(pipe_count, pipes);
	exec->exit_status = wait_for_all_commands(exec, pids, cmd_count);
	if (is_debug_enabled(exec))
		ft_putstr_fd("minishell: pipeline execution complete\n", 2);
}
/* Check if command includes a pipe */
int has_pipe(t_cmd_node *cmd)
{
	if (!cmd)
		return 0;
	return (cmd->type == PIPE);
}
// void	execute_pipe(t_exec *exec, char ***commands, int cmd_count)
// {
// 	char		*input;
// 	t_cmd_node	*pipe_cmds;
// 	int			i;
// 	int			j;
// 	char		*cmd_str;
// 	char		*tmp;

// 	input = NULL;
// 	pipe_cmds = NULL;
// 	i = -1;
// 	while (++i < cmd_count)
// 	{
// 		cmd_str = NULL;
// 		j = -1;
// 		while (commands[i][++j])
// 		{
// 			if (!cmd_str)
// 				cmd_str = ft_strdup(&exec->gc, commands[i][j]);
// 			else
// 			{
// 				tmp = ft_strjoin(cmd_str, " ", &exec->gc);
// 				cmd_str = ft_strjoin(tmp, commands[i][j], &exec->gc);
// 			}
// 		}
// 		if (!input)
// 			input = cmd_str;
// 		else
// 		{
// 			tmp = ft_strjoin(input, " | ", &exec->gc);
// 			input = ft_strjoin(tmp, cmd_str, &exec->gc);
// 		}
// 	}
// 	if (input)
// 		pipe_cmds = parse_piped_commands(input, exec);
// 	if (pipe_cmds)
// 		execute_with_pipes(exec, pipe_cmds);
// }
/* Execute a pipe from string commands (old interface maintained for compatibility) */
static char	*join_command_parts(t_exec *exec, char **parts)
{
	char	*cmd_str;
	char	*tmp;
	int		j;

	cmd_str = NULL;
	j = -1;
	while (parts[++j])
	{
		if (!cmd_str)
			cmd_str = ft_strdup(&exec->gc, parts[j]);
		else
		{
			tmp = ft_strjoin(cmd_str, " ", &exec->gc);
			cmd_str = ft_strjoin(tmp, parts[j], &exec->gc);
		}
	}
	return (cmd_str);
}

static char	*append_command_with_pipe(t_exec *exec, char *input, char *cmd_str)
{
	char	*tmp;

	tmp = ft_strjoin(input, " | ", &exec->gc);
	return (ft_strjoin(tmp, cmd_str, &exec->gc));
}

void	execute_pipe(t_exec *exec, char ***commands, int cmd_count)
{
	char		*input;
	t_cmd_node	*pipe_cmds;
	int			i;
	char		*cmd_str;

	input = NULL;
	pipe_cmds = NULL;
	i = -1;
	while (++i < cmd_count)
	{
		cmd_str = join_command_parts(exec, commands[i]);
		if (!input)
			input = cmd_str;
		else
			input = append_command_with_pipe(exec, input, cmd_str);
	}
	if (input)
		pipe_cmds = parse_piped_commands(input, exec);
	if (pipe_cmds)
		execute_with_pipes(exec, pipe_cmds);
}
