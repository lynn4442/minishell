/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_search.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:12:00 by marvin            #+#    #+#             */
/*   Updated: 2025/05/24 10:49:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*build_command_path(t_exec *exec, const char *dir, const char *cmd)
{
	char	*temp_path;
	char	*full_path;

	temp_path = ft_strjoin(dir, "/", &exec->gc);
	if (!temp_path)
		return (NULL);
	full_path = ft_strjoin(temp_path, cmd, &exec->gc);
	return (full_path);
}

char	*hunt_in_path_dirs(t_exec *exec, const char *expanded_cmd,
	char **path_dirs)
{
	char	*full_path;
	int		i;

	i = 0;
	while (path_dirs[i])
	{
		full_path = build_command_path(exec, path_dirs[i], expanded_cmd);
		if (full_path && check_command_access(full_path))
			return (full_path);
		i++;
	}
	return (NULL);
}
