/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:05:08 by lyoussef          #+#    #+#             */
/*   Updated: 2025/05/24 10:56:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_single_var(t_exec *exec, const char *var_name, int *had_error)
{
	if (!is_valid_var_name(var_name))
	{
		print_unset_error(var_name, exec);
		*had_error = 1;
	}
	else
		unset_env_var(exec, var_name);
}

void	process_all_vars(t_exec *exec, char **args)
{
	int	i;
	int	had_error;

	had_error = 0;
	i = 1;
	while (args[i])
	{
		process_single_var(exec, args[i], &had_error);
		i++;
	}
	exec->exit_status = had_error;
}

void	ft_unset(t_exec *exec, char **args)
{
	if (!exec || !args)
		return ;
	if (!args[1])
	{
		exec->exit_status = 0;
		return ;
	}
	process_all_vars(exec, args);
}
