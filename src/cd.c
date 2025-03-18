/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/18 12:49:26 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

t_env_var *get_env_var(t_exec *exec, const char *key)
{
	t_env_var *current = exec->env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return current;
		current = current->next;
	}
	return NULL;
}

void update_env_var(t_exec *exec, const char *key, const char *value, bool equal)
{
    t_env_var *var = get_env_var(exec, key);
    if (var)
    {
        free(var->value);
        var->value = ft_strdup(value);
        var->equal = equal;
        free(var->all);
        if (equal)
            var->all = ft_strjoin(ft_strjoin(var->key, "="), var->value);
        else
            var->all = ft_strdup(var->key);
    }
    else
    {
        t_env_var *new_var = malloc(sizeof(t_env_var));
        if (!new_var)
        {
            perror("malloc");
            return;
        }
        new_var->key = ft_strdup(key);
        if (!new_var->key) {
            perror("malloc");
            free(new_var);
            return;
        }
        new_var->equal = equal;
        new_var->value = ft_strdup(value);
        if (!new_var->value && equal) {
            perror("malloc");
            free(new_var->key);
            free(new_var);
            return;
        }
        if (equal)
            new_var->all = ft_strjoin(ft_strjoin(new_var->key, "="), new_var->value);
        else
            new_var->all = ft_strdup(new_var->key);

        new_var->next = exec->env_list;
        new_var->prev = NULL;

        if (exec->env_list)
            exec->env_list->prev = new_var;
        exec->env_list = new_var;
    }
}

void update_pwd_vars(t_exec *exec, const char *old_pwd)
{
    char *cwd = NULL;
    size_t len = 1024;

    cwd = (char *)malloc(len * sizeof(char));
    if (!cwd)
    {
        perror("malloc");
        exec->exit_status = 1;
        return;
    }
    if (!getcwd(cwd, len))
    {
        perror("getcwd");
        free(cwd);
        exec->exit_status = 1;
        return;
    }
    update_env_var(exec, "OLDPWD", old_pwd, true);
    update_env_var(exec, "PWD", cwd, true);
    exec->exit_status = 0;
    free(cwd);
}

int change_dir(const char *path, t_exec *exec)
{
	if (!path || *path == '\0')
	{
		printf("cd: OLDPWD not set\n");
		exec->exit_status = 1;
		return 1;
	}
	if (chdir(path) != 0)
	{
		perror("minishell cd:");
		exec->exit_status = 1;
		return 1;
	}
	return 0;
}

int ft_cd(t_exec *exec, const char *arg)
{
    char *old_pwd;
    t_env_var *pwd_var;
    t_env_var *home_var;

    pwd_var = get_env_var(exec, "PWD");
    old_pwd = (pwd_var) ? pwd_var->value : "";
    if (!arg || ft_strcmp(arg, "~") == 0)
    {
        home_var = get_env_var(exec, "HOME");
        if (!home_var || !home_var->value)
        {
            printf("cd: HOME not set\n");
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
            printf("cd: OLDPWD not set\n");
            exec->exit_status = 1;
            return 1;
        }
        printf("%s\n", oldpwd_var->value);
        arg = oldpwd_var->value;
    }
    if (change_dir(arg, exec) == 0)
    {
        update_pwd_vars(exec, old_pwd);
        return 0;
    }
    return 1;
}
