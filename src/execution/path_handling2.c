/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_handling2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 10:22:40 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/02 10:22:53 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*cmd_search_prequel(t_exec *exec, const char *cmd)
{
	char	*expanded_cmd;
	char	*direct_path;

	if (!cmd)
		return (NULL);
	expanded_cmd = process_quotes(cmd, exec->env_list, exec);
	if (!expanded_cmd)
		return (NULL);
	direct_path = is_path_absolute(exec, expanded_cmd);
	if (direct_path)
		return (direct_path);
	return (expanded_cmd);
}

char	*find_command_path(t_exec *exec, const char *cmd)
{
	char	**path_dirs;
	char	*expanded_cmd;

	expanded_cmd = cmd_search_prequel(exec, cmd);
	if (!expanded_cmd || ft_strchr(expanded_cmd, '/'))
		return (expanded_cmd);
	path_dirs = get_path_from_env(exec);
	if (!path_dirs)
		return (try_local_path(exec, expanded_cmd));
	return (hunt_in_path_dirs(exec, expanded_cmd, path_dirs));
}
