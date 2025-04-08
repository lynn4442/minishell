/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 18:25:53 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/21 01:44:05 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_numeric(const char *str)
{
	int i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void ft_exit(char **args, int last_exit_status, t_exec *exec)
{
	(void)last_exit_status;
	printf("exit\n");
	
	// Check if we're in a nested shell by looking at SHLVL
	int shlvl = 1;  // Default value
	t_env_var *current = exec->env_list;
	while (current)
	{
		if (ft_strcmp(current->key, "SHLVL") == 0)
		{
			shlvl = ft_atoi(current->value);
			break;
		}
		current = current->next;
	}
	
	// Only free memory if we're not in a nested shell
	if (shlvl <= 1)
	{
		// Free the memory before exiting
		if (exec)
		{
			ft_free_all(&exec->gc);
		}
	}
	
	if (!args[1])
		exit(0);
	if (is_numeric(args[1]))
	{
		if (args[2])
		{
			printf("exit: too many arguments\n");
			exec->exit_status = 1;
			return;
		}
		exit(ft_atoi(args[1]) % 256);
	}
	else
	{
		printf("exit: %s: numeric argument required\n", args[1]);
		exit(255);
	}
}
