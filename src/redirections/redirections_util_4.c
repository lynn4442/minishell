/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_util_4.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhussein <hhussein@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:14:11 by hhussein          #+#    #+#             */
/*   Updated: 2025/06/06 17:25:38 by hhussein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_attached_redir(char *arg)
{
	if (is_redirection(arg) && arg[0] != '>' && arg[0] != '<')
		return (1);
	if ((arg[0] == '>' || arg[0] == '<') && ft_strlen(arg) > 1)
		return (1);
	return (0);
}

void	preprocess_attached_redirs(t_cmd_node *cmd, char ***args)
{
	int		i;
	char	**processed;

	i = 0;
	processed = *args;
	while (processed[i])
	{
		if (is_attached_redir(processed[i]))
		{
			processed = process_attached_redirections(processed,
					&cmd->exec->gc, &i);
			continue ;
		}
		i++;
	}
	*args = processed;
}

void	process_input_redirection(t_cmd_node *cmd, char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "<") == 0 && args[i + 1])
		{
			cmd->in = args[i + 1];
			i += 2;
		}
		else
			i++;
	}
}

static void	process_output_file(char *file, int append)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	ft_putstr_fd("DEBUG - Creating redirection file: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd("\n", 2);
	fd = open(file, flags, 0644);
	if (fd >= 0)
		close(fd);
}

void	process_output_redirection(t_cmd_node *cmd, char **args)
{
	int		file_count;
	int		*is_append;
	char	**files;
	int		i;

	file_count = 0;
	is_append = NULL;
	files = extract_redirection_files(args, &is_append,
			&file_count, &cmd->exec->gc);
	if (file_count <= 0)
		return ;
	i = 0;
	while (i < file_count)
	{
		process_output_file(files[i], is_append[i]);
		i++;
	}
	cmd->out = files[file_count - 1];
	cmd->append = is_append[file_count - 1];
}
