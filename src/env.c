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

#include "../minishell.h"

char *get_env_value(t_env_var *env_list, char *var_name)
{
	t_env_var *current;

	if (!env_list || !var_name)
		return (NULL);

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

t_env_var *get_env_var(t_exec *exec, const char *key)
{
	t_env_var *current;

	if (!exec || !key)
		return (NULL);

	current = exec->env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void init_env(t_exec *exec, char **envp)
{
	int i;
	char *env_var;
	char *equal_sign;
	char *key;
	char *value;
	size_t key_len;

	if (!exec || !envp)
		return;

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

	if (!exec)
		return (NULL);

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
		exec->exit_status = 1;
		return;
	}

	current = exec->env_list;
	while (current)
	{
		if (current->value)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	exec->exit_status = 0;
}

void update_shlvl(t_exec *exec)
{
	t_env_var *shlvl_var;
	char *new_value;
	int current_level;

	shlvl_var = get_env_var(exec, "SHLVL");
	if (shlvl_var && shlvl_var->value)
	{
		current_level = ft_atoi(shlvl_var->value);
		current_level++;
		new_value = ft_itoa(current_level, &exec->gc);
		if (new_value)
			add_or_update_env_var(&exec->gc, &exec->env_list, "SHLVL", new_value);
	}
	else
		add_or_update_env_var(&exec->gc, &exec->env_list, "SHLVL", "1");
}

