/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhussein <hhussein@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:43:55 by hhussein          #+#    #+#             */
/*   Updated: 2025/06/03 22:39:37 by hhussein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_input_error(t_cmd_node *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->in, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	cmd->exec->exit_status = 1;
}

static void	handle_input_file(int fd, int prev_pipe_fd)
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

void	setup_pipe_input(t_cmd_node *cmd, int prev_pipe_fd)
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
		handle_input_file(fd, prev_pipe_fd);
		return ;
	}
	if (prev_pipe_fd != -1)
		handle_pipe_input(prev_pipe_fd);
}

void	redirect_to_pipe(int next_pipe_fd)
{
	if (dup2(next_pipe_fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		exit(1);
	}
	close(next_pipe_fd);
}
