/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:05:08 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/22 13:56:24 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env_var *remove_env_var(t_exec *exec, const char *name)
{
	t_env_var *current;
	t_env_var *prev;

	if (!exec || !name || !exec->env_list)
		return NULL;

	current = exec->env_list;
	prev = NULL;
	while (current)
	{
		// Direct comparison without quote handling
		if (ft_strcmp(current->key, name) == 0)
		{
			if (prev == NULL)
			{
				exec->env_list = current->next;
				if (exec->env_list)
					exec->env_list->prev = NULL;
			}
			else
			{
				prev->next = current->next;
				if (current->next)
					current->next->prev = prev;
			}
			current->next = NULL;
			current->prev = NULL;
			return current;
		}
		prev = current;
		current = current->next;
	}
	return NULL;
}

// Helper function for error messages
static void print_unset_error(const char *var_name, t_exec *exec)
{
	ft_putstr_fd("minishell: unset: `", 2);
	ft_putstr_fd(var_name, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	exec->exit_status = 1;
}

void unset_env_var(t_exec *exec, const char *name)
{
	t_env_var *node_to_remove;

	if (!exec || !name)
		return;

	if (!is_valid_var_name(name))
	{
		print_unset_error(name, exec);
		return;
	}

	node_to_remove = remove_env_var(exec, name);
	if (node_to_remove)
	{
		if (node_to_remove->key)
			free(node_to_remove->key);
		if (node_to_remove->value)
			free(node_to_remove->value);
		if (node_to_remove->all)
			free(node_to_remove->all);
		free(node_to_remove);
	}
}

void ft_unset(t_exec *exec, char **args)
{
	int i;
	int had_error;

	if (!exec || !args)
		return;

	if (!args[1])
	{
		exec->exit_status = 0;
		return;
	}

	had_error = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_var_name(args[i]))
		{
			print_unset_error(args[i], exec);
			had_error = 1;
		}
		else
			unset_env_var(exec, args[i]);
		i++;
	}

	exec->exit_status = had_error ? 1 : 0;
}
