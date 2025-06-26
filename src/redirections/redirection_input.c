/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_input.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:01:27 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/26 15:01:27 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"

static int	open_input_file(t_cmd_node *cmd)
{
	int	fd;

	if (!cmd->in)
		return (0);
	fd = open(cmd->in, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->in, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (-1);
	}
	return (fd);
}

static int	redirect_input(int fd, int *original_fd)
{
	*original_fd = dup(STDIN_FILENO);
	if (*original_fd == -1)
	{
		perror("minishell");
		close(fd);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell");
		close(fd);
		close(*original_fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	setup_input_redirection(t_cmd_node *cmd, int *original_fd)
{
	int	fd;

	fd = open_input_file(cmd);
	if (fd <= 0)
		return (fd);
	if (redirect_input(fd, original_fd) == -1)
		return (-1);
	return (fd);
}

int	restore_input_redirection(int original_fd)
{
	if (original_fd != -1)
	{
		if (dup2(original_fd, STDIN_FILENO) == -1)
		{
			perror("minishell");
			close(original_fd);
			return (-1);
		}
		close(original_fd);
	}
	return (0);
}
