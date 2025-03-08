/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:39:22 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/08 19:29:45 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *get_env_value(t_env_var *env_list, char *var_name)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->name, var_name) == 0)
			return env_list->value;
		env_list = env_list->next;
	}
	return "(unknown)";
}

void print_arg(char *arg, t_env_var *env)
{
	char *env_value;

	if (!arg)
		return ;
	if (arg[0] == '$' && arg[1] != '\0')
	{
		env_value = get_env_value(env, arg + 1);
		if (env_value)
			write(1, env_value, ft_strlen(env_value));
	}
	else
		write(1, arg, ft_strlen(arg));
}

void ft_echo(t_cmd_node *cmd, t_env_var *env, t_exec *exec)
{
	int i;
	int no_newline;

	i = 1;
	no_newline = 0;
	if (!cmd || !cmd->arr || !cmd->arr[0])
		return;
	while (cmd->arr[i] && ft_strcmp(cmd->arr[i], "-n") == 0)
	{
		no_newline = 1;
		i++;
	}
	while (cmd->arr[i])
	{
		print_arg(cmd->arr[i], env);
		if (cmd->arr[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!no_newline)
		write(1, "\n", 1);
	exec->exit_status = 0;
}
