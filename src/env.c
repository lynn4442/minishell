/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:06:43 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/08 15:12:15 by lyoussef         ###   ########.fr       */
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
		if (current->value)
			printf("%s=%s\n", current->name, current->value);
		else
			printf("%s=\n", current->name);
		current = current->next;
	}
}

int main()
{
	t_exec exec;
	exec.env_list = NULL;

	add_env_var(&exec, "USER", "john");
	add_env_var(&exec, "HOME", "/home/john");
	add_env_var(&exec, "SHELL", "/bin/bash");
	add_env_var(&exec, "EMPTY", "");
	add_env_var(&exec, "NULLVALUE", NULL);
	printf("Environment Variables:\n");
	ft_env(&exec);
	return 0;
}
