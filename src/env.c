/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:06:43 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/21 01:38:35 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

void init_env(t_exec *exec, char **envp)
{
	int i;
	char *env_var;
	char *equal_sign;
	char *key;
	char *value;
	size_t key_len;

	i = 0;
	while (envp[i])
	{
		env_var = envp[i];
		equal_sign = ft_strchr(env_var, '=');
		if (equal_sign)
		{
			key_len = equal_sign - env_var;
			key = ft_strndup(&exec->gc, env_var, key_len);
			value = ft_strdup(&exec->gc, equal_sign + 1);
			if (key && value)
				handle_export(&exec->gc, &exec->env_list, env_var);
		}
		i++;
	}
}

char **get_path_from_env(t_exec *exec)
{
	t_env_var *current;

	current = exec->env_list;
	while (current)
	{
		if (ft_strcmp(current->key, "PATH") == 0)
			return (ft_split(current->value, ':', &exec->gc));
		current = current->next;
	}
	return (NULL);
}

void ft_env(t_exec *exec, char **args)
{
	t_env_var *current;

	if (!exec || !exec->env_list)
		return;
	if (args && args[1])
	{
		printf("env: '%s': No such file or directory\n", args[1]);
		return;
	}
	current = exec->env_list;
	while (current)
	{
		if (current->value)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}

