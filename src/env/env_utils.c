/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 18:23:43 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/26 18:23:43 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

char	*get_env_value(t_env_var *env_list, char *var_name)
{
	t_env_var	*current;

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

t_env_var	*get_env_var(t_exec *exec, const char *key)
{
	t_env_var	*current;

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

char	**get_path_from_env(t_exec *exec)
{
	t_env_var	*current;

	if (!exec)
		return (NULL);
	current = exec->env_list;
	while (current)
	{
		if (ft_strcmp(current->key, "PATH") == 0)
		{
			if (current->value && ft_strlen(current->value) > 0)
				return (ft_split(current->value, ':', &exec->gc));
			else
				return (NULL);
		}
		current = current->next;
	}
	return (NULL);
}
