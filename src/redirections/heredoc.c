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

static	int	create_heredoc_temp_file(char *f_name)
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

static	char	*process_heredoc_line(const char *line,	t_env_var *env,
				t_exec *exec)
{
	return (process_heredoc_quotes(line, env, exec));
}

static	int	read_heredoc_content(const char *delimiter, t_env_var *env,
			t_exec *exec, int fd)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		expanded = process_heredoc_line(line, env, exec);
		if (!expanded)
		{
			free(line);
			return (-1);
		}
		write(fd, expanded, ft_strlen(expanded));
		write(fd, "\n", 1);
		free(line);
	}
	return (0);
}

int	handle_heredoc(t_cmd_node *cmd, t_exec *exec)
{
	char	*f_name;
	char	*index;
	int		fd;
	int		read;
	int		i;

	if (!cmd->heredoc_delimiter)
		return (0);
	i = 0;
	while (cmd->heredoc_delimiter[i])
	{
		index = ft_itoa(exec->heredoc_counter, &exec->gc);
		if (!index)
			return (-1);
		f_name = ft_strjoin("/tmp/heredoc_", index, &exec->gc);
		if (!f_name)
			return (-1);
		fd = create_heredoc_temp_file(f_name);
		if (fd == -1)
			return (-1);
		read = read_heredoc_content(cmd->heredoc_delimiter[i],
				exec->env_list, exec, fd);
		if (read == -1)
			return (-1);
		close(fd);
		//printf("%s", f_name);
		cmd->in = f_name;
		exec->heredoc_counter++;
		i++;
	}
	return (0);
}

void cleanup_heredoc_files(t_cmd_node *cmd)
{
	if (!cmd || !cmd->in)
		return ;
	if (strncmp(cmd->in, "/tmp/heredoc_", 14) == 0)
	{
		if (unlink(cmd->in) == -1)
			ft_putstr_fd("error removing file", 2);
	}
}
