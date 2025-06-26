/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 18:23:51 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/26 18:23:51 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

t_env_var	*create_env_var(t_exec *exec, char *name, char *value)
{
	t_env_var	*env;

	env = ft_malloc(&exec->gc, sizeof(t_env_var));
	if (!env)
		return (NULL);
	env->key = ft_strdup(&exec->gc, name);
	env->value = ft_strdup(&exec->gc, value);
	if (!env->key || !env->value)
		return (NULL);
	env->next = NULL;
	env->prev = NULL;
	return (env);
}

void	add_env_var(t_exec *exec, char *name, char *value)
{
	t_env_var	*new_var;
	t_env_var	*temp;

	new_var = create_env_var(exec, name, value);
	if (!new_var)
		return ;
	if (!exec->env_list)
		exec->env_list = new_var;
	else
	{
		temp = exec->env_list;
		while (temp->next)
			temp = temp->next;
		temp->next = new_var;
		new_var->prev = temp;
	}
}

void	update_env_var(t_gc *gc, t_env_var *var, const char *value)
{
	if (value)
		var->value = ft_strdup(gc, value);
	else
		var->value = NULL;
	var->equal = (value != NULL);
	if (value)
		var->all = ft_strjoin(ft_strjoin(var->key, "=", gc), value, gc);
	else
		var->all = ft_strdup(gc, var->key);
}

t_env_var	*create_new_env_var(t_gc *gc, const char *name, const char *value)
{
	t_env_var	*new_var;

	new_var = ft_malloc(gc, sizeof(t_env_var));
	if (!new_var)
		return (NULL);
	new_var->key = ft_strdup(gc, name);
	if (value)
		new_var->value = ft_strdup(gc, value);
	else
		new_var->value = NULL;
	new_var->equal = (value != NULL);
	if (value)
		new_var->all = ft_strjoin(ft_strjoin(new_var->key, "=", gc), value, gc);
	else
		new_var->all = ft_strdup(gc, new_var->key);
	new_var->next = NULL;
	new_var->prev = NULL;
	return (new_var);
}

void	add_or_update_env_var(t_gc *gc, t_env_var **env_list, const char *name,
	const char *value)
{
	t_env_var	*temp;
	t_env_var	*new_var;

	temp = *env_list;
	while (temp)
	{
		if (ft_strcmp(temp->key, name) == 0)
		{
			update_env_var(gc, temp, value);
			return ;
		}
		temp = temp->next;
	}
	new_var = create_new_env_var(gc, name, value);
	if (!new_var)
		return ;
	new_var->next = *env_list;
	if (*env_list)
		(*env_list)->prev = new_var;
	*env_list = new_var;
}
