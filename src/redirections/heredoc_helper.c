/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhussein <hhussein@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 21:10:56 by hhussein          #+#    #+#             */
/*   Updated: 2025/06/25 21:43:51 by hhussein         ###   ########.fr       */
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

static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_signal_received = 130;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static int	handle_heredoc_interrupt(void (*old_handler)(int), char *line)
{
	signal(SIGINT, old_handler);
	if (line)
		free(line);
	return (130);
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
	void	(*old_handler)(int);
	int		status;

	old_handler = signal(SIGINT, heredoc_sigint_handler);
	g_signal_received = 0;
	while (1)
	{
		line = readline("> ");
		if (!line || g_signal_received == 130)
			return (handle_heredoc_interrupt(old_handler, line));
		status = process_and_write_heredoc_line(line, delimiter, exec, fd);
		if (status == 1)
			break ;
		if (status == -1)
		{
			signal(SIGINT, old_handler);
			return (-1);
		}
	}
	signal(SIGINT, old_handler);
	return (0);
}
