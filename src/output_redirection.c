/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 22:00:00 by marvin            #+#    #+#             */
/*   Updated: 2025/05/24 22:00:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Opens output file with appropriate flags trunc|append and returns fd */
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

/* Opens output file with appropriate flags based on append mode */
static int	open_output_file(t_cmd_node *cmd)
{
	int	fd;

	if (cmd->append)
		fd = open(cmd->out, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(cmd->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		perror("minishell");
	return (fd);
}

/* Sets up output redirection by opening file,
saving original stdout, and redirecting */
int	setup_output_redirection(t_cmd_node *cmd, int *original_fd)
{
	int	fd;

	if (!cmd->out)
		return (0);
	fd = open_output_file(cmd);
	if (fd == -1)
		return (-1);
	if (save_original_fd(STDOUT_FILENO, original_fd) == -1)
	{
		close(fd);
		return (-1);
	}
	if (redirect_fd(fd, STDOUT_FILENO, *original_fd) == -1)
		return (-1);
	close(fd);
	return (fd);
}

/* Restores stdout to its original state 
using the saved file descriptor */
int	restore_output_redirection(int original_fd)
{
	return (restore_fd_redirection(original_fd, STDOUT_FILENO));
}
