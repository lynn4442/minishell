/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:43:55 by hhussein          #+#    #+#             */
/*   Updated: 2025/06/25 13:14:40 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipes.h"

static void	print_input_error(t_cmd_node *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->in, 2);
	ft_putstr_fd(": No such file or directoryQWDQFQFQQ\n", 2);
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
	int fd;

    if (cmd->in && cmd->in[0] != '\0')
    {
        fd = open(cmd->in, O_RDONLY);
        if (fd == -1)
        {
            print_input_error(cmd);
            if (prev_pipe_fd != -1)
                close(prev_pipe_fd);
			ft_free_all(&cmd->exec->gc);
            exit(1);
        }
        handle_input_file(fd, prev_pipe_fd);
        return;
    }
    if (prev_pipe_fd != -1)
        handle_pipe_input(prev_pipe_fd);
}

void	redirect_to_pipe(int next_pipe_fd, t_gc *gc)
{
	if (dup2(next_pipe_fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		ft_free_all(gc);
		exit(1);
	}
	close(next_pipe_fd);
}
