/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 22:00:00 by marvin            #+#    #+#             */
/*   Updated: 2025/05/24 22:00:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Opens input file for reading and handles error reporting */
static int	open_input_file(t_cmd_node *cmd)
{
	int	fd;

	fd = open(cmd->in, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->in, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
	}
	return (fd);
}

/* Sets up input redirection by opening file,
saving original stdin, and redirecting */
int	setup_input_redirection(t_cmd_node *cmd, int *original_fd)
{
	int	fd;

	if (!cmd->in)
		return (0);
	fd = open_input_file(cmd);
	if (fd == -1)
		return (-1);
	if (save_original_fd(STDIN_FILENO, original_fd) == -1)
	{
		close(fd);
		return (-1);
	}
	if (redirect_fd(fd, STDIN_FILENO, *original_fd) == -1)
		return (-1);
	close(fd);
	return (fd);
}

/* Restores stdin to its original state using the saved file descriptor */
int	restore_input_redirection(int original_fd)
{
	return (restore_fd_redirection(original_fd, STDIN_FILENO));
}
