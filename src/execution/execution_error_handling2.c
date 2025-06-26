/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_error_handling2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:10:20 by hhussein          #+#    #+#             */
/*   Updated: 2025/06/26 15:01:05 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	print_error(const char *filename, const char *error_msg)
{
	ft_putstr_fd("minishell: ", 2);
	if (filename && *filename)
		ft_putstr_fd(filename, 2);
	ft_putstr_fd(error_msg, 2);
}

int	validate_directory_for_write(t_exec *exec, const char *filename)
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

int	validate_file_exists(t_exec *exec, const char *filename)
{
	if (access(filename, F_OK) != 0)
	{
		print_error(filename, ": No such file or directory\n");
		exec->exit_status = 1;
		return (1);
	}
	return (0);
}

int	validate_file_readable(t_exec *exec, const char *filename)
{
	if (access(filename, R_OK) != 0)
	{
		print_error(filename, ": Permission denied\n");
		exec->exit_status = 1;
		return (1);
	}
	return (0);
}

int	validate_not_directory_for_read(t_exec *exec, const char *filename)
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
