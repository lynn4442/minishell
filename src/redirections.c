/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 19:55:04 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/22 16:03:17 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// heredoc on its own to take care of

# include "../minishell.h"

// <
void ft_input_redirection(t_cmd_node *cmd, t_gc *gc)
{
	if (cmd->in)
	{
		int fd = open(cmd->in, O_RDONLY);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->in, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			ft_free_all(gc);
			exit(1);
		}
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("minishell");
			close(fd);
			ft_free_all(gc);
			exit(1);
		}
		close(fd);
	}
}

// >>
void ft_output_append(t_cmd_node *cmd, t_gc *gc)
{
	if (cmd->out && cmd->append)
	{
		int fd = open(cmd->out, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(cmd->out);
			ft_free_all(gc);
			exit(1);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("minishell");
			close(fd);
			ft_free_all(gc);
			exit(1);
		}
		close(fd);
	}
}

// >
void ft_output_truncate(t_cmd_node *cmd, t_gc *gc)
{
	if (cmd->out && !cmd->append)
	{
		int fd = open(cmd->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(cmd->out);
			ft_free_all(gc);
			exit(1);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("minishell");
			close(fd);
			ft_free_all(gc);
			exit(1);
		}
		close(fd);
	}
}

void handle_redirection(t_cmd_node *cmd, t_gc *gc)
{
	ft_input_redirection(cmd, gc);
	ft_output_append(cmd, gc);
	ft_output_truncate(cmd, gc);
}
