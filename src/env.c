/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:06:43 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/17 15:49:55 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

void ft_env(t_exec *exec)
{
	t_env_var *current;

	if (!exec || !exec->env_list)
		return ;
	current = exec->env_list;
	while (current)
	{
		if (current->value && *current->value != '\0')
			printf("%s=%s\n", current->name, current->value);
		else if (current->value)
			printf("%s=\n", current->name);
		current = current->next;
	}
}
