/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_extra.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 18:28:33 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/26 18:28:33 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"

int	finalize_heredoc_parent(pid_t pid, char *f_name,
			t_exec *exec, t_cmd_node *cmd)
{
	int	status;
	int	res;

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
	int	i;
	int	status;

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
	if (ft_strncmp(cmd->in, "/tmp/heredoc_", 14) == 0)
	{
		if (unlink(cmd->in) == -1)
			ft_putstr_fd("error removing file", 2);
	}
}
