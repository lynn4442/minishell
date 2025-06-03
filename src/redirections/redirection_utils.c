/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 09:18:08 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"

int	handle_output_redirection(t_cmd_node *cmd)
{
	int	fd;

	if (!cmd->out)
		return (STDOUT_FILENO);
	if (cmd->append)
		fd = open(cmd->out, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(cmd->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell");
		return (-1);
	}
	return (fd);
}

/* Process command arguments and extract redirection operators */
void	process_and_update_args(t_cmd_node *cmd, char **args)
{
	int		i;
	int		j;
	char	**new_args;
	int		arg_count;

	i = 0;
	j = 0;
	arg_count = 0;
	while (args[i])
	{
		if ((ft_strcmp(args[i], ">") == 0
				|| ft_strcmp(args[i], ">>") == 0
				|| ft_strcmp(args[i], "<") == 0
				|| ft_strcmp(args[i], "<<") == 0) && args[i + 1])
		{
			i += 2;
		}
		else
		{
			arg_count++;
			i++;
		}
	}
	new_args = ft_malloc(&cmd->exec->gc, sizeof(char *) * (arg_count + 1));
	if (!new_args)
		return ;
	i = 0;
	j = 0;
	while (args[i])
	{
		if ((ft_strcmp(args[i], ">") == 0
				|| ft_strcmp(args[i], ">>") == 0
				|| ft_strcmp(args[i], "<") == 0
				|| ft_strcmp(args[i], "<<") == 0) && args[i + 1])
		{
			i += 2;
			continue ;
		}
		new_args[j++] = ft_strdup(&cmd->exec->gc, args[i++]);
	}
	new_args[j] = NULL;
	cmd->arr = new_args;
}

/* Setup output redirection and save original stdout */
int	setup_output_redirection(t_cmd_node *cmd, int *original_fd)
{
	int	fd;

	if (!cmd->out)
		return (0);
	if (cmd->append)
		fd = open(cmd->out, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(cmd->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell");
		return (-1);
	}
	*original_fd = dup(STDOUT_FILENO);
	if (*original_fd == -1)
	{
		perror("minishell");
		close(fd);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell");
		close(fd);
		close(*original_fd);
		return (-1);
	}
	close(fd);
	return (fd);
}

/* Restore stdout to its original state */
int	restore_output_redirection(int original_fd)
{
	if (original_fd != -1)
	{
		if (dup2(original_fd, STDOUT_FILENO) == -1)
		{
			perror("minishell");
			close(original_fd);
			return (-1);
		}
		close(original_fd);
	}
	return (0);
}

/* Setup input redirection and save original stdin */
int	setup_input_redirection(t_cmd_node *cmd, int *original_fd)
{
	int	fd;

	if (!cmd->in)
		return (0);
	fd = open(cmd->in, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->in, 2);
		ft_putstr_fd(": No such file or directory6\n", 2);
		return (-1);
	}
	*original_fd = dup(STDIN_FILENO);
	if (*original_fd == -1)
	{
		perror("minishell");
		close(fd);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell");
		close(fd);
		close(*original_fd);
		return (-1);
	}
	close(fd);
	return (fd);
}

/* Restore stdin to its original state */
int	restore_input_redirection(int original_fd)
{
	if (original_fd != -1)
	{
		if (dup2(original_fd, STDIN_FILENO) == -1)
		{
			perror("minishell");
			close(original_fd);
			return (-1);
		}
		close(original_fd);
	}
	return (0);
}
