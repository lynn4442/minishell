/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:06:43 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/20 16:26:34 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

void ft_env(t_exec *exec, char **args)
{
	t_env_var *current;
	int i = 1;

	if (!exec || !exec->env_list)
		return;
	while (args && args[i])
	{
		char *equal_sign = ft_strchr(args[i], '=');
		if (equal_sign)
		{
			char *key = ft_strndup(args[i], equal_sign - args[i]);
			char *value = ft_strdup(equal_sign + 1);
			if (key)
			{
				update_env_var(exec, key, value, true);
				free(key);
				free(value);
			}
		}
		else
		{
			printf("env: '%s': No such file or directory\n", args[i]);
			return;
		}
		i++;
	}
	current = exec->env_list;
	while (current)
	{
		if (current->value)
			printf("%s=%s\n", current->key, current->value);
		else
			printf("%s=\n", current->key);
		current = current->next;
	}
}

