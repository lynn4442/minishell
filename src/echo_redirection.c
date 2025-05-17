/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 00:17:43 by marvin            #+#    #+#             */
/*   Updated: 2025/05/18 00:17:43 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_echo_redirection(t_cmd_node *cmd, int *original_stdout, t_exec *exec)
{
	int	fd;

	fd = handle_output_redirection(cmd);
	if (fd == -1)
	{
		exec->exit_status = 1;
		return (-1);
	}
	if (fd != STDOUT_FILENO)
	{
		*original_stdout = dup(STDOUT_FILENO);
		if (*original_stdout == -1 || dup2(fd, STDOUT_FILENO) == -1)
		{
			ft_putstr_fd("minishell: echo: redirection error\n", 2);
			if (*original_stdout != -1)
				close(*original_stdout);
			close(fd);
			exec->exit_status = 1;
			return (-1);
		}
		close(fd);
	}
	return (0);
}

int	restore_echo_output(int original_stdout, t_exec *exec)
{
	if (original_stdout != -1)
	{
		if (dup2(original_stdout, STDOUT_FILENO) == -1)
		{
			ft_putstr_fd("minishell: echo: failed to restore stdout\n", 2);
			exec->exit_status = 1;
			return (-1);
		}
		close(original_stdout);
	}
	return (0);
}
