/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:39:22 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/22 15:15:16 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *get_env_value(t_env_var *env_list, char *var_name)
{
	t_env_var *current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
			return current->value;
		current = current->next;
	}
	printf("Environment variable not found.\n");
	return (NULL);
}

void print_arg(char *arg, t_env_var *env, t_exec *exec)
{
	char *env_value;
	char *exit_code_str;

	if (!arg)
		return;
	if (arg[0] == '$' && arg[1] != '\0')
	{
		if (arg[1] == '?')
		{
			exit_code_str = ft_itoa(exec->exit_status, &exec->gc);
			printf("%s", exit_code_str);
			//heyde chou lezim aamoul fiya eza free aade aw ft_free_all
			free(exit_code_str);
			exec->exit_status = 0;
		}
		else
		{
			env_value = get_env_value(env, arg + 1);
			if (env_value)
				printf("%s", env_value);
		}
	}
	else
		printf("%s", arg);
}

void ft_echo(t_cmd_node *cmd, t_env_var *env, t_exec *exec)
{
	int i;
	int no_newline;

	i = 1;
	no_newline = 0;
	if (!cmd || !cmd->arr || !cmd->arr[0])
		return;
	while (cmd->arr[i] && cmd->arr[i][0] == '-' && cmd->arr[i][1] == 'n')
	{
		int j = 1;
		while (cmd->arr[i][j] == 'n')
			j++;
		if (cmd->arr[i][j] != '\0')
			break;
		no_newline = 1;
		i++;
	}
	while (cmd->arr[i])
	{
		print_arg(cmd->arr[i], env, exec);
		if (cmd->arr[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!no_newline)
		printf("\n");
	exec->exit_status = 0;
}

