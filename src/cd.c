/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/08 17:30:43 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

t_env_var *get_env_var(t_exec *exec, const char *name)
{
	t_env_var *current = exec->env_list;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
			return current;
		current = current->next;
	}
	return NULL;
}

void update_env_var(t_exec *exec, const char *name, const char *value)
{
	t_env_var *var = get_env_var(exec, name);
	if (var)
	{
		free(var->value);
		var->value = ft_strdup(value);
	}
	else
	{
		t_env_var *new_var = malloc(sizeof(t_env_var));
		new_var->name = ft_strdup(name);
		new_var->value = ft_strdup(value);
		new_var->next = exec->env_list;
		new_var->prev = NULL;
		if (exec->env_list)
			exec->env_list->prev = new_var;
		exec->env_list = new_var;
	}
}

int change_dir(const char *path, t_exec *exec)
{
	if (!path || *path == '\0')
	{
		write(2, "cd: OLDPWD not set\n", 19);
		exec->exit_status = 1;
		return 1;
	}
	if (chdir(path) != 0)
	{
		perror("minishell");
		exec->exit_status = 1;
		return 1;
	}
	return 0;
}

void update_pwd_vars(t_exec *exec, const char *old_pwd)
{
	char cwd[1024];
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("cd");
		exec->exit_status = 1;
		return;
	}
	update_env_var(exec, "OLDPWD", old_pwd);
	update_env_var(exec, "PWD", cwd);
	exec->exit_status = 0;
}

int ft_cd(t_exec *exec, const char *arg)
{
	char		*old_pwd;
	t_env_var	*pwd_var;
	t_env_var *home_var;

	pwd_var = get_env_var(exec, "PWD");
	if (pwd_var)
	  old_pwd = pwd_var->value;
	else
	  old_pwd = "";
	if (!arg || ft_strcmp(arg, "~") == 0)
	{
		home_var = get_env_var(exec, "HOME");
		if (!home_var || !home_var->value)
		{
			write(2, "cd: HOME not set\n", 17);
			exec->exit_status = 1;
			return 1;
		}
		arg = home_var->value;
	}
	else if (ft_strcmp(arg, "-") == 0)
	{
		t_env_var *oldpwd_var = get_env_var(exec, "OLDPWD");
		if (!oldpwd_var || !oldpwd_var->value)
		{
			write(2, "cd: OLDPWD not set\n", 19);
			exec->exit_status = 1;
			return 1;
		}
		printf("%s\n", oldpwd_var->value);
		arg = oldpwd_var->value;
	}
	if (change_dir(arg, exec) == 0)
	{
		update_pwd_vars(exec, old_pwd);
		return (0);
	}
	return (1);
}
