/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 18:16:48 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/09 18:32:20 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *get_current_directory(void)
{
	char *buf = malloc(1024);
	if (!buf)
		return (NULL);
	if (!getcwd(buf, 1024))
	{
		free(buf);
		return (NULL);
	}
	return buf;
}

void ft_pwd(t_exec *exec)
{
	char *cwd = get_current_directory();
	if (cwd)
	{
		ft_putstr_fd(cwd, 1);
		ft_putstr_fd("\n", 1);
		free(cwd);
	}
	else
	{
		t_env_var *pwd_var = get_env_var(exec, "PWD");
		if (pwd_var && pwd_var->value)
		{
			ft_putstr_fd(pwd_var->value, 1);
			ft_putstr_fd("\n", 1);
		}
		else
		{
			ft_putstr_fd("pwd: error retrieving current directory\n", 2);
			exec->exit_status = 1;
			return;
		}
	}
	exec->exit_status = 0;
}
