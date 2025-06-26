/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 19:55:04 by lyoussef          #+#    #+#             */
/*   Updated: 2025/05/04 17:30:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

int	get_shell_level(t_exec *exec)
{
	int			shlvl;
	t_env_var	*current;

	shlvl = 1;
	current = exec->env_list;
	while (current)
	{
		if (ft_strcmp(current->key, "SHLVL") == 0)
		{
			if (current->value && ft_strlen(current->value) > 0)
				shlvl = ft_atoi(current->value);
			break ;
		}
		current = current->next;
	}
	return (shlvl);
}

void	update_shlvl(t_exec *exec)
{
	t_env_var	*shlvl_var;
	char		*new_value;
	int			current_level;

	shlvl_var = get_env_var(exec, "SHLVL");
	if (shlvl_var && shlvl_var->value)
	{
		if (ft_strlen(shlvl_var->value) == 0 || !is_numeric(shlvl_var->value))
		{
			add_or_update_env_var(&exec->gc, &exec->env_list, "SHLVL", "1");
			return ;
		}
		current_level = ft_atoi(shlvl_var->value);
		if (current_level < 0)
			current_level = 0;
		current_level++;
		new_value = ft_itoa(current_level, &exec->gc);
		if (new_value)
			add_or_update_env_var(&exec->gc, &exec->env_list,
				"SHLVL", new_value);
	}
	else
		add_or_update_env_var(&exec->gc, &exec->env_list, "SHLVL", "1");
}
