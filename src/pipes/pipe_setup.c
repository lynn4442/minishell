/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_setup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:47:37 by hhussein          #+#    #+#             */
/*   Updated: 2025/06/25 22:35:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipes.h"

static void	setup_pipe_output(t_cmd_node *cmd, int next_pipe_fd)
{
	int	flags;
	int	file_fd;

	if (cmd->out)
	{
		flags = O_WRONLY | O_CREAT;
		if (cmd->append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		file_fd = open(cmd->out, flags, 0644);
		if (file_fd == -1)
			print_output_error(cmd);
		redirect_to_file(file_fd, &cmd->exec->gc);
		if (next_pipe_fd != -1)
			close(next_pipe_fd);
	}
	else if (next_pipe_fd != -1)
		redirect_to_pipe(next_pipe_fd, &cmd->exec->gc);
}

int	create_all_pipes(int pipe_count, int pipes[][2])
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe");
			close_all_pipes(i, pipes);
			return (0);
		}
		i++;
	}
	return (1);
}

void	setup_child_pipes(t_cmd_node *current, int i,
	int pipe_count, int pipes[][2])
{
	if (i > 0)
		setup_pipe_input(current, pipes[i - 1][0]);
	else
		setup_pipe_input(current, -1);
	if (i < pipe_count)
		setup_pipe_output(current, pipes[i][1]);
	else
		setup_pipe_output(current, -1);
}
