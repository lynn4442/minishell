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

static int	child_read_heredoc_content(const char *delimiter, t_exec *exec, int fd)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		line = readline("> ");
		if (!line)
			return (130); // EOF (Ctrl+D) treated like interrupt for heredoc
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

int	create_and_read_heredoc(char *delimiter, t_exec *exec, t_cmd_node *cmd)
{
	char	*f_name;
	int		fd;
	pid_t	pid;
	int		status;
	int		res;

	fd = prepare_heredoc_file(&f_name, exec);
	if (fd == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(fd);
		unlink(f_name);
		return (-1);
	}
	if (pid == 0)
	{
		setup_heredoc_child_signals();
		int child_ret = child_read_heredoc_content(delimiter, exec, fd);
		close(fd);
		if (child_ret == 130)
			exit(130);
		if (child_ret == -1)
			exit(1);
		exit(0);
	}
	close(fd);
	waitpid(pid, &status, 0);
	res = process_heredoc_status(status, f_name, exec);
	if (res != 0)
		return (res);
	cmd->in = f_name;
	exec->heredoc_counter++;
	return (0);
}

int	handle_heredoc(t_cmd_node *cmd, t_exec *exec)
{
	int		i;
	int		status;

	if (!cmd->heredoc_delimiter)
		return (0);
	i = 0;
	while (cmd->heredoc_delimiter[i])
	{
		status = create_and_read_heredoc(cmd->heredoc_delimiter[i], exec, cmd);
		if (status != 0)
			return (status);
		i++;
	}
	return (0);
}

void	cleanup_heredoc_files(t_cmd_node *cmd)
{
	if (!cmd || !cmd->in)
		return ;
	if (strncmp(cmd->in, "/tmp/heredoc_", 14) == 0)
	{
		if (unlink(cmd->in) == -1)
			ft_putstr_fd("error removing file", 2);
	}
}
