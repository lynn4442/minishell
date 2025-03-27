/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 09:46:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/27 09:44:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int handle_builtin_command(t_exec *exec, t_cmd_node *cmd)
{
    if (!exec || !cmd || !cmd->arr || !cmd->arr[0])
        return (0);

    if (ft_strcmp(cmd->arr[0], "echo") == 0)
    {
        ft_echo(cmd, exec->env_list, exec);
        return (1);
    }
    else if (ft_strcmp(cmd->arr[0], "cd") == 0)
    {
        ft_cd(exec, cmd->arr[1]);
        return (1);
    }
    else if (ft_strcmp(cmd->arr[0], "pwd") == 0)
    {
        ft_pwd(exec);
        return (1);
    }
    else if (ft_strcmp(cmd->arr[0], "env") == 0)
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
    else if (ft_strcmp(cmd->arr[0], "exit") == 0)
    {
        ft_exit(cmd->arr, exec->exit_status);
        return (1);
    }
    return (0);
} 