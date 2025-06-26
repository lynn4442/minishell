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

static int	validate_filename(t_exec *exec, const char *filename)
{
	if (!filename || !*filename)
	{
		print_error("", ": No such file or directory\n");
		exec->exit_status = 1;
		return (1);
	}
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
