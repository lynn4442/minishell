/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_core.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 09:18:08 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cd.h"

int	cd_change_directory(const char *path, t_exec *exec)
{
	if (!path || *path == '\0')
	{
		printf("cd: OLDPWD not set\n");
		exec->exit_status = 1;
		return (1);
	}
	if (access(path, F_OK) != 0)
	{
		printf("minihell: cd: %s: No such file or directory\n", path);
		exec->exit_status = 1;
		return (1);
	}
	if (access(path, X_OK) != 0)
	{
		printf("minihell: cd: %s: Permission denied\n", path);
		exec->exit_status = 1;
		return (1);
	}
	if (chdir(path) != 0)
	{
		printf("minihell: cd: %s: Not a directory\n", path);
		exec->exit_status = 1;
		return (1);
	}
	return (0);
}

int	cd_validate_args(t_cmd_node *cmd, t_exec *exec)
{
	if (cmd && cmd->arr[2] != NULL)
	{
		ft_putstr_fd("bash: cd: too many arguments\n", 2);
		exec->exit_status = 1;
		return (1);
	}
	return (0);
}

char	*cd_get_current_pwd(t_exec *exec)
{
	t_env_var	*pwd_var;

	pwd_var = get_env_var(exec, "PWD");
	if (pwd_var)
		return (pwd_var->value);
	return ("");
}

const char	*cd_resolve_path(t_exec *exec, t_cmd_node *cmd,
	char **expanded_path)
{
	const char	*arg;
	const char	*path;

	arg = NULL;
	if (cmd)
		arg = cmd->arr[1];
	if (!arg || ft_strcmp(arg, "~") == 0 || ft_strcmp(arg, "--") == 0)
		path = cd_get_home_path(exec);
	else if (ft_strncmp(arg, "~/", 2) == 0)
	{
		*expanded_path = cd_expand_home_path(exec, arg);
		path = *expanded_path;
	}
	else if (ft_strcmp(arg, "-") == 0)
		path = cd_get_oldpwd(exec);
	else
		path = arg;
	return (path);
}

int	ft_cd(t_exec *exec, t_cmd_node *cmd)
{
	char		*old_pwd;
	char		*expanded_path;
	const char	*path;

	if (cd_validate_args(cmd, exec))
		return (1);
	old_pwd = cd_get_current_pwd(exec);
	expanded_path = NULL;
	path = cd_resolve_path(exec, cmd, &expanded_path);
	if (!path)
		return (1);
	if (cd_change_directory(path, exec) == 0)
		cd_update_pwd_vars(exec, old_pwd);
	return (exec->exit_status);
}
