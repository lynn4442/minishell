/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 09:18:08 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pwd.h"

char	*get_current_directory(t_exec *exec)
{
	char	*buf;

	buf = ft_malloc(&exec->gc, 1024);
	if (!buf)
		return (NULL);
	if (!getcwd(buf, 1024))
	{
		return (NULL);
	}
	return (buf);
}

void	ft_pwd(t_exec *exec)
{
	char		*cwd;
	t_env_var	*pwd_var;

	cwd = get_current_directory(exec);
	if (cwd)
	{
		ft_putstr_fd(cwd, 1);
		ft_putstr_fd("\n", 1);
	}
	else
	{
		pwd_var = get_env_var(exec, "PWD");
		if (pwd_var && pwd_var->value)
		{
			ft_putstr_fd(pwd_var->value, 1);
			ft_putstr_fd("\n", 1);
		}
		else
		{
			ft_putstr_fd("pwd: error retrieving current directory\n", 2);
			exec->exit_status = 1;
			return ;
		}
	}
	exec->exit_status = 0;
}
