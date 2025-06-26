/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 09:18:08 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cd.h"

void	cd_update_pwd_vars(t_exec *exec, const char *old_pwd)
{
	char	*cwd;
	size_t	len;

	cwd = NULL;
	len = 1024;
	cwd = (char *)ft_malloc(&exec->gc, len * sizeof(char));
	if (!cwd)
	{
		perror("malloc");
		exec->exit_status = 1;
		return ;
	}
	if (!getcwd(cwd, len))
	{
		perror("getcwd");
		exec->exit_status = 1;
		return ;
	}
	if (old_pwd)
		add_or_update_env_var(&exec->gc, &exec->env_list, "OLDPWD", old_pwd);
	add_or_update_env_var(&exec->gc, &exec->env_list, "PWD", cwd);
	exec->exit_status = 0;
}

char	*cd_get_home_path(t_exec *exec)
{
	t_env_var	*home_var;

	home_var = get_env_var(exec, "HOME");
	if (!home_var || !home_var->value)
	{
		printf("cd: HOME not set\n");
		exec->exit_status = 1;
		return (NULL);
	}
	return (home_var->value);
}

char	*cd_expand_home_path(t_exec *exec, const char *arg)
{
	char	*home;
	char	*expanded;

	home = cd_get_home_path(exec);
	if (!home)
		return (NULL);
	expanded = ft_strjoin(home, arg + 1, &exec->gc);
	if (!expanded)
	{
		perror("malloc");
		exec->exit_status = 1;
	}
	return (expanded);
}

char	*cd_get_oldpwd(t_exec *exec)
{
	t_env_var	*oldpwd_var;

	oldpwd_var = get_env_var(exec, "OLDPWD");
	if (!oldpwd_var || !oldpwd_var->value)
	{
		printf("cd: OLDPWD not set\n");
		exec->exit_status = 1;
		return (NULL);
	}
	printf("%s\n", oldpwd_var->value);
	return (oldpwd_var->value);
}
