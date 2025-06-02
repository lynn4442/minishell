/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/02 03:32:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

int	is_builtin_command(const char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	handle_echo_cd_pwd(t_exec *exec, t_cmd_node *cmd)
{
	if (ft_strcmp(cmd->arr[0], "echo") == 0)
	{
		ft_echo(cmd, exec->env_list, exec);
		return (1);
	}
	else if (ft_strcmp(cmd->arr[0], "cd") == 0)
	{
		ft_cd(exec, cmd);
		return (1);
	}
	else if (ft_strcmp(cmd->arr[0], "pwd") == 0)
	{
		ft_pwd(exec);
		return (1);
	}
	return (0);
}

int	handle_env_export_unset(t_exec *exec, t_cmd_node *cmd)
{
	if (ft_strcmp(cmd->arr[0], "env") == 0)
	{
		ft_env(exec, cmd->arr);
		return (1);
	}
	else if (ft_strcmp(cmd->arr[0], "export") == 0)
	{
		execute_export(cmd, exec);
		return (1);
	}
	else if (ft_strcmp(cmd->arr[0], "unset") == 0)
	{
		ft_unset(exec, cmd->arr);
		return (1);
	}
	return (0);
}

int	handle_exit(t_exec *exec, t_cmd_node *cmd)
{
	if (ft_strcmp(cmd->arr[0], "exit") == 0)
	{
		ft_exit(cmd->arr, exec->exit_status, exec);
		return (1);
	}
	return (0);
}

int	handle_builtin_command(t_exec *exec, t_cmd_node *cmd)
{
	if (!exec || !cmd || !cmd->arr || !cmd->arr[0])
		return (0);
	if (handle_echo_cd_pwd(exec, cmd))
		return (1);
	if (handle_env_export_unset(exec, cmd))
		return (1);
	if (handle_exit(exec, cmd))
		return (1);
	return (0);
}
