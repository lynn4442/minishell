/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 18:30:56 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/26 18:30:56 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "unset.h"

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
