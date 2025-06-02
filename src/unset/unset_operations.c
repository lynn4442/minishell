/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 10:27:06 by marvin            #+#    #+#             */
/*   Updated: 2025/05/24 10:27:06 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "unset.h"
#include "minishell.h"

t_env_var	*handle_first_node_removal(t_exec *exec, t_env_var *current)
{
	exec->env_list = current->next;
	if (exec->env_list)
		exec->env_list->prev = NULL;
	current->next = NULL;
	current->prev = NULL;
	return (current);
}

t_env_var	*handle_middle_node_removal(t_env_var *current, t_env_var *prev)
{
	prev->next = current->next;
	if (current->next)
		current->next->prev = prev;
	current->next = NULL;
	current->prev = NULL;
	return (current);
}

t_env_var	*find_and_remove_node(t_exec *exec, const char *name)
{
	t_env_var	*current;
	t_env_var	*prev;

	current = exec->env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, name) == 0)
		{
			if (prev == NULL)
				return (handle_first_node_removal(exec, current));
			else
				return (handle_middle_node_removal(current, prev));
		}
		prev = current;
		current = current->next;
	}
	return (NULL);
}

t_env_var	*remove_env_var(t_exec *exec, const char *name)
{
	if (!exec || !name || !exec->env_list)
		return (NULL);
	return (find_and_remove_node(exec, name));
}
