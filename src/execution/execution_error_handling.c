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

	if (access(command_path, F_OK) != 0)
		return (handle_command_not_found(exec, command_path));
	if (stat(command_path, &stat_buf) != 0)
		return (handle_file_not_found(exec, command_path));
	if (S_ISDIR(stat_buf.st_mode))
		return (handle_is_directory_error(exec, command_path));
	if (access(command_path, X_OK) != 0)
		return (handle_permission_denied(exec, command_path));
	return (0);
}

static void	print_error(const char *filename, const char *error_msg)
{
	ft_putstr_fd("minishell: ", 2);
	if (filename && *filename)
		ft_putstr_fd(filename, 2);
	ft_putstr_fd(error_msg, 2);
}

static int	validate_filename(t_exec *exec, const char *filename)
{
	if (!filename || !*filename)
	{
		print_error("", ": No such file or directory SFFQ\n");
		exec->exit_status = 1;
		return (1);
	}
	return (0);
}

static int	validate_directory_for_write(t_exec *exec, const char *filename)
{
	struct stat	stat_buf;

	if (stat(filename, &stat_buf) == 0 && S_ISDIR(stat_buf.st_mode))
	{
		print_error(filename, ": Is a directory\n");
		exec->exit_status = 1;
		return (1);
	}
	return (0);
}

static int	validate_file_exists(t_exec *exec, const char *filename)
{
	if (access(filename, F_OK) != 0)
	{
		print_error(filename, ": No such file or directory QSCQFQ\n");
		exec->exit_status = 1;
		return (1);
	}
	return (0);
}

static int	validate_file_readable(t_exec *exec, const char *filename)
{
	if (access(filename, R_OK) != 0)
	{
		print_error(filename, ": Permission denied\n");
		exec->exit_status = 1;
		return (1);
	}
	return (0);
}

static int	validate_not_directory_for_read(t_exec *exec, const char *filename)
{
	struct stat	stat_buf;

	if (stat(filename, &stat_buf) == 0 && S_ISDIR(stat_buf.st_mode))
	{
		print_error(filename, ": Is a directory\n");
		exec->exit_status = 1;
		return (1);
	}
	return (0);
}

static int	validate_write_mode(t_exec *exec, const char *filename)
{
	return (validate_directory_for_write(exec, filename));
}

static int	validate_read_mode(t_exec *exec, const char *filename)
{
	if (validate_file_exists(exec, filename))
		return (1);
	if (validate_file_readable(exec, filename))
		return (1);
	if (validate_not_directory_for_read(exec, filename))
		return (1);
	return (0);
}

int	validate_redirection_file(t_exec *exec, const char *filename, int mode)
{
	if (validate_filename(exec, filename))
		return (1);

	if (mode & O_WRONLY)
	{
		if (validate_write_mode(exec, filename))
			return (1);
	}

	if (mode & O_RDONLY)
	{
		if (validate_read_mode(exec, filename))
			return (1);
	}

	return (0);
}

