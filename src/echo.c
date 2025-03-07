/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:39:22 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/06 19:45:30 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_env_value(t_env_var *env_list, char *var_name)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->name, var_name) == 0)
			return env_list->value;
		env_list = env_list->next;
	}
	return "";
}

void	ft_echo(t_cmd_node *cmd, t_env_var *env)
{
	int		i;
	int		newline;
	char	*env_value;

	i = 1;
	newline = 1;
	if (!cmd || !cmd->arr || !cmd->arr[0])
		return ;
	if (cmd->arr[i] && ft_strcmp(cmd->arr[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (cmd->arr[i])
	{
		if (cmd->arr[i][0] == '$')
		{
			env_value = get_env_value(env, cmd->arr[i] + 1);
			write(1, env_value, ft_strlen(env_value));
		}
		else
			write(1, cmd->arr[i], ft_strlen(cmd->arr[i]));

		if (cmd->arr[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
}
