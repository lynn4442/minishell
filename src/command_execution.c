/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 09:46:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/05/14 01:07:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
//find the path of a command
static char	*is_path_absolute(t_exec *exec, const char *expanded_cmd)
{
	if (ft_strchr(expanded_cmd, '/') || ft_strncmp(expanded_cmd, "./", 2) == 0)
		return (ft_strdup(&exec->gc, expanded_cmd));
	// not relative or absolute path we need to check in the PATH
	return (NULL);
}

static char	*try_local_path(t_exec *exec, const char *expanded_cmd)
{
	char	*full_path;

	full_path = ft_strjoin("./", expanded_cmd, &exec->gc);
	if (full_path && access(full_path, F_OK) == 0) // F_OK to make sure that the file exist
		return full_path;
	return (NULL);
}

static char	*hunt_in_path_dirs(t_exec *exec, const char *expanded_cmd, char **path_dirs)
{
	char	*full_path;
	char	*temp_path;
	int		i;

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

static char	*cmd_search_prequel(t_exec *exec, const char *cmd)
{
	char	*expanded_cmd;
	char	*direct_path;

	if (!cmd)
		return (NULL);
	//expand env var in command name
	expanded_cmd = process_quotes(cmd, exec->env_list, exec);
	if (!expanded_cmd)
		return (NULL);
	direct_path = is_path_absolute(exec, expanded_cmd);
	if (direct_path)
		return (direct_path);
	return (expanded_cmd);
}

char	*find_command_path(t_exec *exec, const char *cmd)
{
	char	**path_dirs;
	char	*expanded_cmd;

	expanded_cmd = cmd_search_prequel(exec, cmd);
	if (!expanded_cmd || ft_strchr(expanded_cmd, '/'))
		return (expanded_cmd);
	
	path_dirs = get_path_from_env(exec);
	// in case PATH not set || empty --> only absolute com can run
	if (!path_dirs) 
		return (try_local_path(exec, expanded_cmd));
	return (hunt_in_path_dirs(exec, expanded_cmd, path_dirs));
}

// error handling
static void	report_cmd_failure(t_exec *exec, const char *cmd, int error_type)
{
	if (!cmd)
		return ;
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

static void	restore_og_redirections(int original_in, int original_out)
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

static int	setup_input_redirection_local(t_cmd_node *cmd, int *original_in)
{
	int	fd;
	
	(void)original_in; // Silence unused parameter warning
	
	if (!cmd->in)
		return (0);
		
	fd = open(cmd->in, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->in, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		cmd->exec->exit_status = 1;
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	setup_output_redirection_local(t_cmd_node *cmd, int *original_out)
{
	int	fd;
	int	flags;
	
	(void)original_out; // Silence unused parameter warning
	
	if (!cmd->out)
		return (0);
		
	flags = O_WRONLY | O_CREAT;
	if (cmd->append)
		flags = flags | O_APPEND;
	else
		flags = flags | O_TRUNC;
	fd = open(cmd->out, flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->out, 2);
		ft_putstr_fd(": Error opening output file\n", 2);
		cmd->exec->exit_status = 1;
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	setup_redirections(t_cmd_node *cmd, int *original_in, int *original_out)
{
	*original_in = dup(STDIN_FILENO);
	*original_out = dup(STDOUT_FILENO);
	
	if (setup_input_redirection_local(cmd, original_in) == -1)
	{
		restore_og_redirections(*original_in, *original_out);
		return (-1);
	}
	
	if (setup_output_redirection_local(cmd, original_out) == -1)
	{
		restore_og_redirections(*original_in, *original_out);
		return (-1);
	}
	
	return (0);
}

static int	run_builtin_cmd(t_exec *exec, t_cmd_node *cmd)
{
	return (handle_builtin_command(exec, cmd));
}

static int	handle_lost_command(t_exec *exec, const char *expanded_cmd)
{
	if (ft_strchr(expanded_cmd, '/'))
	{
		// has path but doesn't exist || can't be accessed
		if (access(expanded_cmd, F_OK) != 0)
			report_cmd_failure(exec, expanded_cmd, 3);
		else if (access(expanded_cmd, X_OK) != 0)
			report_cmd_failure(exec, expanded_cmd, 2);
		else
			report_cmd_failure(exec, expanded_cmd, 0);
	}
	else
	{
		// command not found in PATH
		report_cmd_failure(exec, expanded_cmd, 1);
	}
	return (1);
}

static void	launch_child_mission(t_cmd_node *cmd, 
	char *cmd_path, char **env_array)
{
	// Child process
	setup_child_signals();
	execve(cmd_path, cmd->arr, env_array);
	// If execve fails
	ft_putstr_fd("minishell: ", 2);
	perror(cmd->arr[0]);
	exit(126);
}

static void	wait_for_child_return(t_exec *exec, pid_t pid)
{
	int	status;
	
	// Parent process
	waitpid(pid, &status, 0);
	// Update exit status
	if (WIFEXITED(status))
		exec->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exec->exit_status = 128 + WTERMSIG(status);
}

static int	prep_cmd_for_launch(t_exec *exec, t_cmd_node *cmd, 
	char **expanded_cmd, char **cmd_path)
{
	*expanded_cmd = process_quotes(cmd->arr[0], exec->env_list, exec);
	if (!*expanded_cmd)
	{
		report_cmd_failure(exec, cmd->arr[0], 1);
		return (1);
	}
	
	*cmd_path = find_command_path(exec, *expanded_cmd);
	if (!*cmd_path)
		return (handle_lost_command(exec, *expanded_cmd));
	
	if (access(*cmd_path, X_OK) != 0)
	{
		report_cmd_failure(exec, *expanded_cmd, 2);
		return (1);
	}
	
	return (0);
}

static int	execute_external_quest(t_exec *exec, t_cmd_node *cmd)
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
