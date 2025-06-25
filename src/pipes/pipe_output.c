/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_output.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:44:10 by hhussein          #+#    #+#             */
/*   Updated: 2025/06/25 23:17:01 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipes.h"

void	print_output_error(t_cmd_node *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->out, 2);
	ft_putstr_fd(": Error opening output file\n", 2);
	ft_free_all(&cmd->exec->gc);
	exit(1);
}

static	void	handle_tee_child(int file_fd, int next_pipe_fd,
					int read_end, t_gc *gc)
{
	char	buffer[4096];
	ssize_t	n;

	close(read_end + 1);
	while (1)
	{
		n = read(read_end, buffer, sizeof(buffer));
		if (n == 0)
			break ;
		if (write(file_fd, buffer, n) != n)
		{
			perror("write to file");
			ft_free_all(gc);
			exit(1);
		}
		if (write(next_pipe_fd, buffer, n) != n)
		{
			perror("write to pipe");
			ft_free_all(gc);
			exit(1);
		}
	}
	close(read_end);
	close(file_fd);
	close(next_pipe_fd);
	ft_free_all(gc);
	exit(0);
}

static	void	check_error(int *tee_pipe, int file_fd, t_gc *gc)
{
	if (pipe(tee_pipe) == -1)
	{
		perror("pipe");
		close(file_fd);
		ft_free_all(gc);
		exit(1);
	}
}

void	setup_tee_pipe(int file_fd, int next_pipe_fd, t_gc *gc)
{
	int		tee_pipe[2];
	pid_t	tee_pid;

	check_error(tee_pipe, file_fd, gc);
	tee_pid = fork();
	if (tee_pid == -1)
	{
		perror("fork");
		close(file_fd);
		close(tee_pipe[0]);
		close(tee_pipe[1]);
		ft_free_all(gc);
		exit(1);
	}
	if (tee_pid == 0)
		handle_tee_child(file_fd, next_pipe_fd, tee_pipe[0], gc);
	close(tee_pipe[0]);
	if (dup2(tee_pipe[1], STDOUT_FILENO) == -1)
	{
		perror("dup2");
		ft_free_all(gc);
		exit(1);
	}
	close(tee_pipe[1]);
	close(file_fd);
}

void	redirect_to_file(int file_fd, t_gc *gc)
{
	if (dup2(file_fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		ft_free_all(gc);
		exit(1);
	}
	close(file_fd);
}
