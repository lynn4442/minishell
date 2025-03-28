/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 09:18:08 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_env_var(t_exec *exec, const char *key, const char *value, bool equal)
{
	t_env_var	*var;
	t_env_var	*last;
	t_env_var	*new_var;

	var = get_env_var(exec, key);
	if (var)
	{
		var->value = ft_strdup(&exec->gc, value);
		var->equal = equal;
		if (equal)
			var->all = ft_strjoin(ft_strjoin(var->key, "=", &exec->gc),
					var->value, &exec->gc);
		else
			var->all = ft_strdup(&exec->gc, var->key);
	}
	else
	{
		new_var = ft_malloc(&exec->gc, sizeof(t_env_var));
		if (!new_var)
		{
			perror("malloc");
			return ;
		}
		new_var->key = ft_strdup(&exec->gc, key);
		if (!new_var->key)
		{
			perror("malloc");
			return ;
		}
		new_var->equal = equal;
		new_var->value = ft_strdup(&exec->gc, value);
		if (!new_var->value && equal)
		{
			perror("malloc");
			return ;
		}
		if (equal)
			new_var->all = ft_strjoin(ft_strjoin(new_var->key, "=", &exec->gc),
					new_var->value, &exec->gc);
		else
			new_var->all = ft_strdup(&exec->gc, new_var->key);
		new_var->next = NULL;
		if (!exec->env_list)
		{
			new_var->prev = NULL;
			exec->env_list = new_var;
		}
		else
		{
			last = exec->env_list;
			while (last->next)
				last = last->next;
			last->next = new_var;
			new_var->prev = last;
		}
	}
}

void	update_pwd_vars(t_exec *exec, const char *old_pwd)
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
	update_env_var(exec, "OLDPWD", old_pwd, true);
	update_env_var(exec, "PWD", cwd, true);
	exec->exit_status = 0;
}
// no need for all these just to be like bash in this case
// > cd makefile
// > bash: cd: makefile: No such file or directory

int	change_dir(const char *path, t_exec *exec)
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

char *get_home_path(t_exec *exec)
{
	t_env_var *home_var;

	home_var = get_env_var(exec, "HOME");
	if (!home_var || !home_var->value)
	{
		printf("cd: HOME not set\n");
		exec->exit_status = 1;
		return (NULL);
	}
	return (home_var->value);
}

char *expand_home_path(t_exec *exec, const char *arg)
{
	char	*home;
	char	*expanded;

	home = get_home_path(exec);
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

char	*handle_oldpwd(t_exec *exec)
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

int	ft_cd(t_exec *exec, t_cmd_node *cmd)
{
	char *old_pwd;
	t_env_var *pwd_var;
	char *expanded_path;
	const char *path;
	const char *arg = cmd ? cmd->arr[1] : NULL;

	// Check for multiple arguments
	if (cmd && cmd->arr[2] != NULL)
	{
		ft_putstr_fd("bash: cd: too many arguments\n", 2);
		exec->exit_status = 1;
		return 1;
	}

	pwd_var = get_env_var(exec, "PWD");
	old_pwd = pwd_var ? pwd_var->value : "";

	// Handle special cases
	if (!arg || ft_strcmp(arg, "~") == 0 || ft_strcmp(arg, "--") == 0)
		path = get_home_path(exec);
	else if (ft_strncmp(arg, "~/", 2) == 0)
	{
		expanded_path = expand_home_path(exec, arg);
		path = expanded_path;
	}
	else if (ft_strcmp(arg, "-") == 0)
		path = handle_oldpwd(exec);
	else
		path = arg;

	if (!path)
		return 1;

	if (change_dir(path, exec) == 0)
		update_pwd_vars(exec, old_pwd);
	
	return exec->exit_status;
}

