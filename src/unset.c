/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:05:08 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/07 20:33:35 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

t_env_var *remove_env_var(t_exec *exec, const char *name)
{
	t_env_var *current;
	t_env_var *prev;

	current = exec->env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			if (prev == NULL)
			{
				exec->env_list = current->next;
				if (current->next)
					current->next->prev = NULL;
			}
			else if (current->next)
			{
				prev->next = current->next;
				current->next->prev = prev;
			}
			else
				prev->next = NULL;
			return current;
		}
		prev = current;
		current = current->next;
	}
	return NULL;
}

void free_env_var(t_exec *exec, t_env_var *node)
{
	if (node)
	{
		free(node->name);
		free(node->value);
		free(node);
		exec->exit_status = 0;
	}
	else
		exec->exit_status = 0;
}

void unset_env_var(t_exec *exec, const char *name)
{
	t_env_var *node_to_remove;

	node_to_remove = remove_env_var(exec, name);
	free_env_var(exec, node_to_remove);
}
