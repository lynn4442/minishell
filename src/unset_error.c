/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:05:08 by lyoussef          #+#    #+#             */
/*   Updated: 2025/05/24 10:53:28 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_unset_error(const char *var_name, t_exec *exec)
{
	ft_putstr_fd("minishell: unset: `", 2);
	ft_putstr_fd(var_name, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	exec->exit_status = 1;
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
