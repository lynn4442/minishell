/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 10:22:43 by marvin            #+#    #+#             */
/*   Updated: 2025/05/24 10:22:43 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
