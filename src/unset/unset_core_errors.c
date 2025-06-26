/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 10:27:30 by marvin            #+#    #+#             */
/*   Updated: 2025/05/24 10:27:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "unset.h"

void	print_unset_error(const char *var_name, t_exec *exec)
{
	ft_putstr_fd("minishell: unset: `", 2);
	ft_putstr_fd(var_name, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	exec->exit_status = 1;
}

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

void	unset_env_var(t_exec *exec, const char *name)
{
	if (!exec || !name)
		return ;
	if (!is_valid_var_name(name))
	{
		print_unset_error(name, exec);
		return ;
	}
	remove_env_var(exec, name);
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
