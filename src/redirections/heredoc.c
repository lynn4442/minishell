/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/23 01:23:13 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"

static int	prepare_heredoc_file(char **f_name, t_exec *exec)
{
	char	*index;
	int		fd;

	index = ft_itoa(exec->heredoc_counter, &exec->gc);
	if (!index)
		return (-1);
	*f_name = ft_strjoin("/tmp/heredoc_", index, &exec->gc);
	if (!*f_name)
		return (-1);
	fd = create_heredoc_temp_file(*f_name);
	if (fd == -1)
		return (-1);
	return (fd);
}

static int	child_read_heredoc_content(const char *delimiter,
				t_exec *exec, int fd)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		line = readline("> ");
		if (!line)
			return (130);
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		expanded = process_heredoc_quotes(line, exec->env_list, exec);
		free(line);
		if (!expanded)
			return (-1);
		write(fd, expanded, ft_strlen(expanded));
		write(fd, "\n", 1);
	}
	return (0);
}

static int	handle_fork_error(int fd, char *f_name)
{
	close(fd);
	unlink(f_name);
	return (-1);
}

static void	execute_heredoc_child(const char *delimiter,
					t_exec *exec, int fd)
{
	int	child_ret;

	setup_heredoc_child_signals();
	child_ret = child_read_heredoc_content(delimiter, exec, fd);
	close(fd);
	if (child_ret == 130)
		exit(130);
	if (child_ret == -1)
		exit(1);
	exit(0);
}

int	create_and_read_heredoc(char *delimiter,
			t_exec *exec, t_cmd_node *cmd)
{
	char	*f_name;
	int		fd;
	pid_t	pid;

	fd = prepare_heredoc_file(&f_name, exec);
	if (fd == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (handle_fork_error(fd, f_name));
	if (pid == 0)
		execute_heredoc_child(delimiter, exec, fd);
	close(fd);
	return (finalize_heredoc_parent(pid, f_name, exec, cmd));
}
