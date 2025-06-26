/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 21:10:56 by hhussein          #+#    #+#             */
/*   Updated: 2025/06/26 01:57:26 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"

int	create_heredoc_temp_file(char *f_name)
{
	int		fd;

	fd = open(f_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("error creating file", 2);
		return (-1);
	}
	return (fd);
}

static int	process_and_write_heredoc_line(char *line, const char *delimiter,
						t_exec *exec, int fd)
{
	char	*expanded;

	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (1);
	}
	expanded = process_heredoc_quotes(line, exec->env_list, exec);
	if (!expanded)
	{
		free(line);
		return (-1);
	}
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	free(line);
	return (0);
}

int	read_heredoc_content(const char *delimiter, t_exec *exec, int fd)
{
	char	*line;
	int		status;

	while (1)
	{
		line = readline("> ");
		if (!line)
			return (130);
		status = process_and_write_heredoc_line(line, delimiter, exec, fd);
		if (status == 1)
			break ;
		if (status == -1)
			return (-1);
	}
	return (0);
}

void	setup_heredoc_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

int	process_heredoc_status(int status, char *f_name, t_exec *exec)
{
	int	child_exit;

	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write(1, "\n", 1);
		unlink(f_name);
		exec->exit_status = 130;
		return (130);
	}
	if (WIFEXITED (status))
	{
		child_exit = WEXITSTATUS(status);
		if (child_exit == 130)
		{
			write(1, "\n", 1);
			unlink(f_name);
			exec->exit_status = 130;
			return (130);
		}
		else if (child_exit != 0)
			return (unlink(f_name), -1);
	}
	return (0);
}
