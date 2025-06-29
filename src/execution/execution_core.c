/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_core.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 09:18:08 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	expand_command_arguments(t_exec *exec, t_cmd_node *cmd)
{
	int		idx;
	char	*expanded;

	if (!cmd || !cmd->arr)
		return ;
	idx = 0;
	while (cmd->arr[idx])
	{
		expanded = process_quotes(cmd->arr[idx], exec->env_list, exec);
		if (expanded)
			cmd->arr[idx] = expanded;
		idx++;
	}
}

void	execute_command_supreme(t_exec *exec, t_cmd_node *cmd)
{
	expand_command_arguments(exec, cmd);
	int	original_in;
	int	original_out;

	original_in = -1;
	original_out = -1;
	if (!exec || !cmd || !cmd->arr)
		return ;
	if (!cmd->arr[0] && (cmd->in || cmd->out || cmd->heredoc_delimiter))
	{
		execute_command_with_redirect(exec, cmd, original_in, original_out);
		return ;
	}
	if (!cmd->arr[0])
		return ;
	if (cmd->type == PIPE && handle_pipe_execution(exec))
		return ;
	if (is_builtin_command(cmd->arr[0])
		&& handle_builtin_without_redirect(exec, cmd))
		return ;
	execute_command_with_redirect(exec, cmd, original_in, original_out);
}

void	parse_and_execute(t_exec *exec, t_cmd_node *cmd)
{
	if (!exec || !cmd || !cmd->arr)
	{
		ft_putstr_fd("minishell: Error: Invalid command node\n", 2);
		exec->exit_status = 1;
		return ;
	}
	if (!cmd->arr[0] && !(cmd->in || cmd->out))
	{
		ft_putstr_fd("minishell: Error: Invalid command node\n", 2);
		exec->exit_status = 1;
		return ;
	}
	execute_command_supreme(exec, cmd);
}
