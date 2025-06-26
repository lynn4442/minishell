/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 18:24:33 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/26 18:24:33 by lyoussef         ###   ########.fr       */
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
	if (!validate_export_argument(arg, exec))
		return (0);
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

void	handle_export(t_gc *gc, t_env_var **env_list, char *arg)
{
	char	*unquoted_arg;
	char	*key;
	char	*value;
	char	*equal_pos;

	unquoted_arg = ft_strtrim(arg, " \t", gc);
	if (!unquoted_arg)
		return ;
	equal_pos = ft_strchr(unquoted_arg, '=');
	if (equal_pos)
	{
		*equal_pos = '\0';
		key = unquoted_arg;
		value = equal_pos + 1;
		if (key && value)
			add_or_update_env_var(gc, env_list, key, value);
	}
	else
	{
		key = ft_strdup(gc, unquoted_arg);
		if (key)
			add_or_update_env_var(gc, env_list, key, NULL);
	}
}
