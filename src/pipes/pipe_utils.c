/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:44:38 by hhussein          #+#    #+#             */
/*   Updated: 2025/06/25 22:36:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipes.h"

void	close_all_pipes(int pipe_count, int pipes[][2])
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		if (pipes[i][0] >= 0)
			close(pipes[i][0]);
		if (pipes[i][1] >= 0)
			close(pipes[i][1]);
		i++;
	}
}

void	handle_builtin_or_empty(t_cmd_node *cmd)
{
	int	status;

	if (!cmd->arr || !cmd->arr[0])
	{
		ft_free_all(&cmd->exec->gc);
		exit(1);
	}
	if (is_builtin_command(cmd->arr[0]))
	{
		handle_builtin_command(cmd->exec, cmd);
		status = cmd->exec->exit_status;
		ft_free_all(&cmd->exec->gc);
		exit(status);
	}
}

int	count_pipeline_commands(t_cmd_node *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}
