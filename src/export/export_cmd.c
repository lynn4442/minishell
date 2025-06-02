/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 09:47:33 by marvin            #+#    #+#             */
/*   Updated: 2025/05/18 09:47:33 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export.h"

int	display_env_vars(t_exec *exec)
{
	if (exec->env_list)
		ft_export(exec->env_list);
	else
		printf("No environment variables available.\n");
	exec->exit_status = 0;
	return (1);
}

int	process_export_arg(t_exec *exec, char *arg)
{
	if (!is_valid_var_name(arg) && !ft_strchr(arg, '='))
	{
		printf("export: `%s': not a valid identifier\n", arg);
		return (0);
	}
	handle_export(&exec->gc, &exec->env_list, arg);
	return (1);
}

int	process_export_args(t_cmd_node *node, t_exec *exec)
{
	int	i;
	int	all_valid;

	i = 1;
	all_valid = 1;
	while (node->arr[i])
	{
		if (!process_export_arg(exec, node->arr[i]))
			all_valid = 0;
		i++;
	}
	if (all_valid)
		exec->exit_status = 0;
	else
		exec->exit_status = 1;
	return (1);
}

int	execute_export(t_cmd_node *node, t_exec *exec)
{
	if (!node->arr[1])
		return (display_env_vars(exec));
	return (process_export_args(node, exec));
}
