/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_validation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:12:00 by marvin            #+#    #+#             */
/*   Updated: 2025/05/24 10:49:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
