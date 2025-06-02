/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/02 10:27:19 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quotes.h"

int	handle_exit_status(char *res, int len, t_exec *exec)
{
	char	*exit_num;
	int		j;

	exit_num = ft_itoa(exec->exit_status, &exec->gc);
	if (exit_num)
	{
		j = 0;
		while (exit_num[j])
			res[len++] = exit_num[j++];
	}
	return (len);
}

int	handle_variable(const char *str, t_quote_check *st,
					t_env_var *env, t_exec *exec)
{
	int		start;
	int		end;
	char	*var_name;
	char	*var_value;

	start = st->i + 1;
	end = start;
	while (str[end] && (ft_isalnum(str[end]) || str[end] == '_'))
		end++;
	if (end > start)
	{
		var_name = ft_strndup(&exec->gc, str + start, end - start);
		var_value = get_env_value(env, var_name);
		if (var_value)
		{
			while (*var_value)
				st->res[st->len++] = *var_value++;
		}
		st->i = end;
	}
	else
		st->res[st->len++] = str[st->i++];
	return (st->len);
}

int	process_special_chars(const char *str, t_quote_check *st,
							t_env_var *env, t_exec *exec)
{
	if (str[st->i + 1] == '?')
	{
		st->len = handle_exit_status(st->res, st->len, exec);
		st->i += 2;
	}
	else
	{
		st->len = handle_variable(str, st, env, exec);
	}
	return (st->len);
}

char	*extract_var_name(const char *str, int start, int end)
{
	char	*var_name;

	var_name = malloc(end - start + 1);
	if (!var_name)
		return (NULL);
	ft_strncpy(var_name, str + start, end - start);
	var_name[end - start] = '\0';
	return (var_name);
}

int	check_quotes(const char *str)
{
	int		i;
	char	quote_type;

	i = 0;
	quote_type = '\0';
	while (str[i])
	{
		if (str[i] == '\\')
		{
			i++;
			if (str[i])
				i++;
			continue ;
		}
		if ((str[i] == '\'' || str[i] == '"'))
		{
			if (quote_type == '\0')
				quote_type = str[i];
			else if (str[i] == quote_type)
				quote_type = '\0';
		}
		i++;
	}
	return (quote_type == '\0');
}
