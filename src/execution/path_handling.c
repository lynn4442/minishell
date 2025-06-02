/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 09:18:08 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*is_path_absolute(t_exec *exec, const char *expanded_cmd)
{
	if (ft_strchr(expanded_cmd, '/') || ft_strncmp(expanded_cmd, "./", 2) == 0)
		return (ft_strdup(&exec->gc, expanded_cmd));
	return (NULL);
}

char	*try_local_path(t_exec *exec, const char *expanded_cmd)
{
	char	*full_path;

	full_path = ft_strjoin("./", expanded_cmd, &exec->gc);
	if (full_path && access(full_path, F_OK) == 0)
		return (full_path);
	return (NULL);
}

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

char	*check_command_access(const char *full_path)
{
	if (access(full_path, F_OK) == 0)
	{
		if (access(full_path, X_OK) == 0)
			return ((char *)full_path);
		perror("minishell");
	}
	return (NULL);
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
