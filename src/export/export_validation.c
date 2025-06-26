/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_validation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/01/20 10:00:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export.h"

int	is_valid_identifier_char(char c, int is_first)
{
	if (is_first)
		return (ft_isalpha(c) || c == '_');
	return (ft_isalnum(c) || c == '_');
}

int	is_valid_variable_name(const char *name)
{
	int	i;

	if (!name || !*name)
		return (0);
	if (!is_valid_identifier_char(name[0], 1))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!is_valid_identifier_char(name[i], 0))
			return (0);
		i++;
	}
	return (1);
}

static int	validate_key_part(const char *arg, char *equal_pos, t_exec *exec)
{
	char	*key;
	int		result;
	size_t	key_len;

	if (equal_pos)
	{
		key_len = equal_pos - arg;
		key = ft_strndup(&exec->gc, arg, key_len);
	}
	else
		key = ft_strdup(&exec->gc, arg);
	if (!key)
		return (0);
	result = is_valid_variable_name(key);
	if (!result)
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		exec->exit_status = 1;
	}
	return (result);
}

int	validate_export_argument(const char *arg, t_exec *exec)
{
	char	*equal_pos;

	if (!arg || !*arg)
	{
		ft_putstr_fd("minishell: export: `", 2);
		if (arg)
			ft_putstr_fd(arg, 2);
		else
			ft_putstr_fd("", 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		exec->exit_status = 1;
		return (0);
	}
	if (arg[0] == '=')
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		exec->exit_status = 1;
		return (0);
	}
	equal_pos = ft_strchr(arg, '=');
	return (validate_key_part(arg, equal_pos, exec));
}

int	validate_unset_argument(const char *name, t_exec *exec)
{
	if (!is_valid_variable_name(name))
	{
		ft_putstr_fd("minishell: unset: `", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		exec->exit_status = 1;
		return (0);
	}
	return (1);
}
