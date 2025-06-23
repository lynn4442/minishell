/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_error_handling.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/01/20 10:00:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	handle_command_not_found(t_exec *exec, const char *command)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": command not found\n", 2);
	exec->exit_status = 127;
	return (127);
}

int	handle_permission_denied(t_exec *exec, const char *command)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": Permission denied\n", 2);
	exec->exit_status = 126;
	return (126);
}

int	handle_file_not_found(t_exec *exec, const char *filename)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	exec->exit_status = 1;
	return (1);
}

int	handle_is_directory_error(t_exec *exec, const char *name)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": Is a directory\n", 2);
	exec->exit_status = 126;
	return (126);
}

int	validate_command_execution(t_exec *exec, const char *command_path)
{
	struct stat	stat_buf;

	// Check if file exists
	if (access(command_path, F_OK) != 0)
	{
		return (handle_command_not_found(exec, command_path));
	}
	
	// Get file information
	if (stat(command_path, &stat_buf) != 0)
	{
		return (handle_file_not_found(exec, command_path));
	}
	
	// Check if it's a directory
	if (S_ISDIR(stat_buf.st_mode))
	{
		return (handle_is_directory_error(exec, command_path));
	}
	
	// Check if file is executable
	if (access(command_path, X_OK) != 0)
	{
		return (handle_permission_denied(exec, command_path));
	}
	
	return (0);
}

int	validate_redirection_file(t_exec *exec, const char *filename, int mode)
{
	struct stat	stat_buf;

	if (!filename || !*filename)
	{
		ft_putstr_fd("minishell: : No such file or directory\n", 2);
		exec->exit_status = 1;
		return (1);
	}
	
	// For output redirections, check if target is a directory
	if (mode & O_WRONLY)
	{
		if (stat(filename, &stat_buf) == 0 && S_ISDIR(stat_buf.st_mode))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(filename, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			exec->exit_status = 1;
			return (1);
		}
	}
	
	// For input redirections, check if file exists and is readable
	if (mode & O_RDONLY)
	{
		if (access(filename, F_OK) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(filename, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			exec->exit_status = 1;
			return (1);
		}
		if (access(filename, R_OK) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(filename, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			exec->exit_status = 1;
			return (1);
		}
		if (stat(filename, &stat_buf) == 0 && S_ISDIR(stat_buf.st_mode))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(filename, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			exec->exit_status = 1;
			return (1);
		}
	}
	
	return (0);
} 