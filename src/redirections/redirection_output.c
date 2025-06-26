/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_output.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 01:15:30 by marvin            #+#    #+#             */
/*   Updated: 2025/06/23 01:15:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"

int	handle_output_redirection(t_cmd_node *cmd)
{
	int	fd;

	if (!cmd->out)
		return (STDOUT_FILENO);
	if (cmd->append)
		fd = open(cmd->out, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(cmd->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell");
		return (-1);
	}
	return (fd);
}

static int	open_output_file(t_cmd_node *cmd)
{
	int	fd;

	if (!cmd->out)
		return (0);
	if (cmd->append)
		fd = open(cmd->out, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(cmd->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell");
		return (-1);
	}
	return (fd);
}

static int	redirect_output(int fd, int *original_fd)
{
	*original_fd = dup(STDOUT_FILENO);
	if (*original_fd == -1)
	{
		perror("minishell");
		close(fd);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell");
		close(fd);
		close(*original_fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	setup_output_redirection(t_cmd_node *cmd, int *original_fd)
{
	int	fd;

	fd = open_output_file(cmd);
	if (fd <= 0)
		return (fd);
	if (redirect_output(fd, original_fd) == -1)
		return (-1);
	return (fd);
}

int	restore_output_redirection(int original_fd)
{
	if (original_fd != -1)
	{
		if (dup2(original_fd, STDOUT_FILENO) == -1)
		{
			perror("minishell");
			close(original_fd);
			return (-1);
		}
		close(original_fd);
	}
	return (0);
}
