/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:05:08 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/21 06:08:41 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

t_env_var *remove_env_var(t_exec *exec, const char *name)
{
	t_env_var *current;
	t_env_var *prev;

	current = exec->env_list;
	prev = NULL;
	while (current) {
		if (ft_strcmp(current->key, name) == 0)
		{
			if (prev == NULL)
			{
				exec->env_list = current->next;
				if (current->next)
				{
					current->next->prev = NULL;
				}
			}
			else
			{
				prev->next = current->next;
				if (current->next)
				{
					current->next->prev = prev;
				}
			}
			return current;
		}
		prev = current;
		current = current->next;
	}
	return NULL;
}

void unset_env_var(t_exec *exec, const char *name) {
	t_env_var *node_to_remove = remove_env_var(exec, name);

	if (node_to_remove)
	{
		ft_free(&exec->gc, node_to_remove->key);
		ft_free(&exec->gc, node_to_remove->value);
		ft_free(&exec->gc, node_to_remove->all);
		ft_free(&exec->gc, node_to_remove);
	}
}
