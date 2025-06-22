/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_core.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 19:55:04 by lyoussef          #+#    #+#             */
/*   Updated: 2025/05/04 17:30:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

// External function declaration
void	handle_export(t_gc *gc, t_env_var **env_list, char *arg);

static void	parse_and_add_env_var(t_exec *exec, char *env_var)
{
	char	*equal_sign;

	equal_sign = ft_strchr(env_var, '=');
	if (equal_sign)
		handle_export(&exec->gc, &exec->env_list, env_var);
}

static void	setup_default_env_vars(t_exec *exec)
{
	char		*cwd;
	t_env_var	*pwd_var;

	pwd_var = get_env_var(exec, "PWD");
	if (!pwd_var)
	{
		cwd = ft_malloc(&exec->gc, 1024);
		if (cwd && getcwd(cwd, 1024))
			add_or_update_env_var(&exec->gc, &exec->env_list, "PWD", cwd);
	}
	add_or_update_env_var(&exec->gc, &exec->env_list, "_", "/usr/bin/env");
}

void	init_env(t_exec *exec, char **envp)
{
	int	i;

	if (!exec || !envp)
		return ;
	i = 0;
	while (envp[i])
		parse_and_add_env_var(exec, envp[i++]);
	setup_default_env_vars(exec);
}

void	ft_env(t_exec *exec, char **args)
{
	t_env_var	*current;

	if (!exec || !exec->env_list)
		return ;
	if (args && args[1])
	{
		printf("env: '%s': No such file or directory 1\n", args[1]);
		exec->exit_status = 1;
		return ;
	}
	current = exec->env_list;
	while (current)
	{
		if (current->value)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	exec->exit_status = 0;
}

void	update_underscore_var(t_exec *exec, const char *last_command)
{
	if (!exec || !last_command)
		return ;
	add_or_update_env_var(&exec->gc, &exec->env_list, "_", last_command);
}
