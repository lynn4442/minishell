/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:45:55 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 12:48:28 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
