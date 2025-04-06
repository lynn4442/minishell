/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:45:55 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 12:48:28 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	swap_env_vars(t_env_var *a, t_env_var *b)
{
	char	*temp_name;
	char	*temp_value;

	temp_name = a->key;
	a->key = b->key;
	b->key = temp_name;

	temp_value = a->value;
	a->value = b->value;
	b->value = temp_value;
}

void	sort_env_vars(t_env_var *head)
{
	int			swapped;
	t_env_var	*current;
	t_env_var	*last_unsorted;

	last_unsorted = NULL;
	swapped = 1;
	if (head == NULL)
		return ;
	while (swapped)
	{
		swapped = 0;
		current = head;
		while (current->next != last_unsorted)
		{
			if (ft_strcmp(current->key, current->next->key) > 0)
			{
				swap_env_vars(current, current->next);
				swapped = 1;
			}
			current = current->next;
		}
		last_unsorted = current;
	}
}

void add_or_update_env_var(t_gc *gc, t_env_var **env_list, char *name, char *value)
{
	t_env_var *temp;
	t_env_var *new_var;

	temp = *env_list;
	while (temp)
	{
		if (ft_strcmp(temp->key, name) == 0)
		{
			if (temp->value)
				free(temp->value);
			if (temp->all)
				free(temp->all);
			temp->value = value ? ft_strdup(gc, value) : NULL;
			temp->equal = (value != NULL);
			if (value)
				temp->all = ft_strjoin(ft_strjoin(temp->key, "=", gc), value, gc);
			else
				temp->all = ft_strdup(gc, temp->key);
			return;
		}
		temp = temp->next;
	}

	new_var = ft_malloc(gc, sizeof(t_env_var));
	if (!new_var)
		return;

	new_var->key = ft_strdup(gc, name);
	new_var->value = value ? ft_strdup(gc, value) : NULL;
	new_var->equal = (value != NULL);
	// Set the all field without quotes
	if (value)
		new_var->all = ft_strjoin(ft_strjoin(new_var->key, "=", gc), value, gc);
	else
		new_var->all = ft_strdup(gc, new_var->key);

	new_var->next = *env_list;
	new_var->prev = NULL;
	if (*env_list)
		(*env_list)->prev = new_var;
	*env_list = new_var;
}

void ft_export(t_env_var *env_list)
{
	t_env_var *temp;

	if (env_list == NULL)
	{
		printf("No environment variables to export.\n");
		return;
	}
	temp = env_list;
	sort_env_vars(env_list);

	while (temp)
	{
		if (temp->key == NULL)
		{
			printf("Error: Found environment variable with NULL key.\n");
			return;
		}
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(temp->key, 1);

		// Case 1: Has a value (like g=h)
		if (temp->value && ft_strcmp(temp->value, "") != 0)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(temp->value, 1);
			ft_putstr_fd("\"", 1);
		}
		// Case 2: Has = but empty value (like f=)
		else if (temp->value)
		{
			ft_putstr_fd("=\"\"", 1);
		}
		// Case 3: No = at all (like d)
		else
		{
			// Don't print anything after the key
		}
		ft_putstr_fd("\n", 1);
		temp = temp->next;
	}
}

int ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\r' || c == '\v' || c == '\f');
}

void handle_export(t_gc *gc, t_env_var **env_list, char *arg)
{
	char *key;
	char *value;
	char *equal_sign;
	size_t key_len;
	char *unquoted_arg;

	// First, strip any quotes from the argument
	unquoted_arg = ft_strtrim(arg, "\"'");
	if (!unquoted_arg)
		return;

	equal_sign = ft_strchr(unquoted_arg, '=');
	if (equal_sign)
	{
		key_len = equal_sign - unquoted_arg;
		key = ft_strndup(gc, unquoted_arg, key_len);
		if (*(equal_sign + 1) == '\0')
			value = ft_strdup(gc, "");
		else
			value = ft_strdup(gc, equal_sign + 1);
		if (key && value)
			add_or_update_env_var(gc, env_list, key, value);
	}
	else
	{
		key = ft_strdup(gc, unquoted_arg);
		if (key)
			add_or_update_env_var(gc, env_list, key, NULL);
	}

	// Since ft_strtrim allocates new memory, we need to free it
	free(unquoted_arg);
}

int execute_export(t_cmd_node *node, t_exec *exec)
{
	int	i;
	int	invalid;

	invalid = 0;

	if (!node->arr[1])
	{
		if (exec->env_list)
			ft_export(exec->env_list);
		else
			printf("No environment variables available.\n");
		exec->exit_status = 0;
		return (1);
	}
	i = 1;
	while (node->arr[i])
	{
		if (!is_valid_var_name(node->arr[i]) && !ft_strchr(node->arr[i], '='))
		{
			printf("export: `%s': not a valid identifier\n", node->arr[i]);
			invalid = 1;
		}
		else
			handle_export(&exec->gc, &exec->env_list, node->arr[i]);
		i++;
	}
	if (invalid)
		exec->exit_status = 1;
	else
		exec->exit_status = 0;
	return 1;
}
