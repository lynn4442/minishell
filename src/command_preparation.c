/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_preparation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:12:00 by marvin            #+#    #+#             */
/*   Updated: 2025/05/17 17:12:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	report_cmd_failure(t_exec *exec, const char *cmd, int error_type)
{
	if (!cmd)
		return ;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	if (error_type == 1)
	{
		ft_putstr_fd(": command not found\n", 2);
		exec->exit_status = 127;
	}
	else if (error_type == 2)
	{
		ft_putstr_fd(": Permission denied\n", 2);
		exec->exit_status = 1;
	}
	else if (error_type == 3)
	{
		ft_putstr_fd(": No such file or directory\n", 2);
		exec->exit_status = 1;
	}
	else
	{
		ft_putstr_fd(": Error executing command\n", 2);
		exec->exit_status = 1;
	}
}

int	handle_lost_command(t_exec *exec, const char *expanded_cmd)
{
	if (ft_strchr(expanded_cmd, '/'))
	{
		if (access(expanded_cmd, F_OK) != 0)
			report_cmd_failure(exec, expanded_cmd, 3);
		else if (access(expanded_cmd, X_OK) != 0)
			report_cmd_failure(exec, expanded_cmd, 2);
		else
			report_cmd_failure(exec, expanded_cmd, 0);
	}
	else
		report_cmd_failure(exec, expanded_cmd, 1);
	return (1);
}

int	prep_cmd_for_launch(t_exec *exec, t_cmd_node *cmd,
	char **expanded_cmd, char **cmd_path)
{
	*expanded_cmd = process_quotes(cmd->arr[0], exec->env_list, exec);
	if (!*expanded_cmd)
	{
		report_cmd_failure(exec, cmd->arr[0], 1);
		return (1);
	}
	*cmd_path = find_command_path(exec, *expanded_cmd);
	if (!*cmd_path)
		return (handle_lost_command(exec, *expanded_cmd));
	if (access(*cmd_path, X_OK) != 0)
	{
		report_cmd_failure(exec, *expanded_cmd, 2);
		return (1);
	}
	return (0);
} 