/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_display.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 18:24:46 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/26 18:24:46 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export.h"

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

void	print_env_var(t_env_var *var)
{
	ft_putstr_fd("declare -x ", 1);
	ft_putstr_fd(var->key, 1);
	if (var->value && ft_strcmp(var->value, "") != 0)
	{
		ft_putstr_fd("=\"", 1);
		ft_putstr_fd(var->value, 1);
		ft_putstr_fd("\"", 1);
	}
	else if (var->value)
	{
		ft_putstr_fd("=\"\"", 1);
	}
	ft_putstr_fd("\n", 1);
}

int	check_env_list(t_env_var *env_list)
{
	if (env_list == NULL)
	{
		printf("No environment variables to export.\n");
		return (0);
	}
	return (1);
}

void	ft_export(t_env_var *env_list)
{
	t_env_var	*temp;

	if (!check_env_list(env_list))
		return ;
	temp = env_list;
	sort_env_vars(env_list);
	while (temp)
	{
		if (temp->key == NULL)
		{
			printf("Error: Found environment variable with NULL key.\n");
			return ;
		}
		print_env_var(temp);
		temp = temp->next;
	}
}
