/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:45:55 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/17 13:37:39 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

void swap_env_vars(t_env_var *a, t_env_var *b)
{
	char *temp_name;
	char *temp_value;

	temp_name = a->name;
	a->name = b->name;
	b->name = temp_name;

	temp_value = a->value;
	a->value = b->value;
	b->value = temp_value;
}

void sort_env_vars(t_env_var *head)
{
	int swapped;
	t_env_var *ptr1;
	t_env_var *lptr;

	lptr = NULL;
	if (head == NULL)
		return;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		ptr1 = head;

		while (ptr1->next != lptr)
		{
			if (ft_strcmp(ptr1->name, ptr1->next->name) > 0)
			{
				swap_env_vars(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	}
}

void add_or_update_env_var(t_env_var **env_list, char *name, char *value)
{
	t_env_var *temp = *env_list;

	while (temp)
	{
		if (ft_strcmp(temp->name, name) == 0)
		{
			free(temp->value);
			if (value)
				temp->value = ft_strdup(value);
			else
				temp->value = NULL;
			//printf("Updated: %s=%s\n", temp->name, temp->value);
			return;
		}
		temp = temp->next;
	}
	t_env_var *new_var = malloc(sizeof(t_env_var));
	new_var->name = ft_strdup(name);
	new_var->value = value ? ft_strdup(value) : NULL;
	new_var->next = *env_list;
	*env_list = new_var;
}

void ft_export(t_env_var *env_list)
{
	t_env_var *temp;
	if (env_list == NULL)
		return;
	//treke l sort abel ma taamle iteration aal list
	sort_env_vars(env_list);

	temp = env_list;
	while (temp)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(temp->name, 1);
		if (temp->value)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(temp->value, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putstr_fd("\n", 1);
		temp = temp->next;
	}
}

int is_valid_var_name(const char *name)
{
	if (!name || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (0);
	for (int i = 1; name[i]; i++)
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
	}
	return (1);
}

void handle_export(char *cmd, t_env_var **env_list)
{
	char *equal_sign;

	equal_sign = ft_strchr(cmd, '=');
	if (!equal_sign)
	{
		if (!is_valid_var_name(cmd))
			return;
		add_or_update_env_var(env_list, cmd, NULL);
		return;
	}
	*equal_sign = '\0';
	char *name = cmd;
	char *value = equal_sign + 1;

	if (!is_valid_var_name(name))
		return;

	add_or_update_env_var(env_list, name, value);
}
