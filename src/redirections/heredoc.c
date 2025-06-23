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
#include "signals.h"

extern int	g_signal_received;

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

static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_signal_received = 130;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static	int	read_heredoc_content(const char *delimiter, t_env_var *env,
			t_exec *exec, int fd)
{
	char	*line;
	char	*expanded;
	void	(*old_handler)(int);

	// Set up heredoc-specific signal handling
	old_handler = signal(SIGINT, heredoc_sigint_handler);
	g_signal_received = 0;
	
	while (1)
	{
		line = readline("> ");
		if (!line || g_signal_received == 130)
		{
			// Ctrl+C was pressed or EOF
			if (g_signal_received == 130)
			{
				signal(SIGINT, old_handler);
				if (line)
					free(line);
				return (130);
			}
			// EOF without Ctrl+C - this is an error in heredoc
			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", 2);
			ft_putstr_fd(delimiter, 2);
			ft_putstr_fd("')\n", 2);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		expanded = process_heredoc_line(line, env, exec);
		if (!expanded)
		{
			free(line);
			signal(SIGINT, old_handler);
			return (-1);
		}
		write(fd, expanded, ft_strlen(expanded));
		write(fd, "\n", 1);
		free(line);
	}
	
	// Restore original signal handler
	signal(SIGINT, old_handler);
	return (0);
}

int	handle_heredoc(t_cmd_node *cmd, t_exec *exec)
{
	char	*f_name;
	char	*index;
	int		fd;
	int		read_result;
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
		read_result = read_heredoc_content(cmd->heredoc_delimiter[i],
				exec->env_list, exec, fd);
		close(fd);
		if (read_result == 130)
		{
			// Ctrl+C was pressed - clean up and return with signal exit status
			unlink(f_name);
			exec->exit_status = 130;
			return (130);
		}
		if (read_result == -1)
			return (-1);
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
