/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:45:55 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/07 21:14:04 by lyoussef         ###   ########.fr       */
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

void export_no_options(t_env_var *env_list)
{
	t_env_var *temp;

	temp = env_list;
	sort_env_vars(env_list);
	while (temp)
	{
		ft_putstr_fd(temp->name, 1);
		ft_putstr_fd("=", 1);
		ft_putstr_fd(temp->value, 1);
		ft_putstr_fd("\n", 1);
		temp = temp->next;
	}
}
