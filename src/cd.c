/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/07 20:42:10 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

int handle_cd_argument(t_exec *exec, const char **arg)
{
	char		*old_pwd;
	t_env_var	*pwd_var;
	t_env_var	*home_var;
	t_env_var	*oldpwd_var;

	pwd_var = get_env_variable(exec, "PWD");
	if (pwd_var)
		old_pwd = pwd_var->value;
	else
		old_pwd = "";
	if (!*arg || ft_strcmp(*arg, "~") == 0)
	{
		home_var = get_env_variable(exec, "HOME");
		if (!home_var || !home_var->value)
		{
			write(2, "cd: HOME not set\n", 17);
			exec->exit_status = 1;
			return 0;
		}
		*arg = home_var->value;
	}
	else if (ft_strcmp(*arg, "-") == 0)
	{
		oldpwd_var = get_env_variable(exec, "OLDPWD");
		if (!oldpwd_var || !oldpwd_var->value)
		{
			write(2, "cd: OLDPWD not set\n", 18);
			exec->exit_status = 1;
			return 0;
		}
		printf("%s\n", oldpwd_var->value);
		*arg = oldpwd_var->value;
	}
	return (1);
}

int ft_cd(t_exec *exec, const char *arg)
{
	char *old_pwd;
	t_env_var *pwd_var;

	if (!handle_cd_argument(exec, &arg))
		return (1);
	pwd_var = get_env_variable(exec, "PWD");
	if (pwd_var)
		old_pwd = pwd_var->value;
	else
		old_pwd = "";
	if (change_directory(arg, exec) == 0)
		update_pwd_vars(exec, old_pwd);
	return (1);
}
