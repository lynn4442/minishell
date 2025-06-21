/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handling.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhussein <hhussein@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/20 17:28:00 by hhussein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"

void	restore_og_redirections(int original_in, int original_out)
{
	if (original_in != -1)
	{
		dup2(original_in, STDIN_FILENO);
		close(original_in);
	}
	if (original_out != -1)
	{
		dup2(original_out, STDOUT_FILENO);
		close(original_out);
	}
}

int	setup_input_redirection_local(t_cmd_node *cmd, int *original_in)
{
	int	fd;

	(void)original_in;
	if (!cmd->in)
		return (0);
	if(cmd->heredoc_delimiter)
	{
		handle_heredoc(cmd, cmd->exec);
	}
	fd = open(cmd->in, O_RDONLY);
	if (fd == -1)
	{
		// ft_putstr_fd("minishell: ", 2);
		// ft_putstr_fd(cmd->in, 2);
		// ft_putstr_fd(": No such file or directory5\n", 2);
		cmd->exec->exit_status = 1;
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	setup_output_redirection_local(t_cmd_node *cmd, int *original_out)
{
	int	fd;
	int	flags;

	(void)original_out;
	if (!cmd->out)
		return (0);
	flags = O_WRONLY | O_CREAT;
	if (cmd->append)
		flags = flags | O_APPEND;
	else
		flags = flags | O_TRUNC;
	fd = open(cmd->out, flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->out, 2);
		ft_putstr_fd(": Error opening output file\n", 2);
		cmd->exec->exit_status = 1;
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	setup_redirections(t_cmd_node *cmd, int *original_in, int *original_out)
{
	*original_in = dup(STDIN_FILENO);
	*original_out = dup(STDOUT_FILENO);
	if (setup_input_redirection_local(cmd, original_in) == -1)
	{
		restore_og_redirections(*original_in, *original_out);
		return (-1);
	}
	if (setup_output_redirection_local(cmd, original_out) == -1)
	{
		restore_og_redirections(*original_in, *original_out);
		return (-1);
	}
	return (0);
}
