/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/26 01:53:05 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quotes.h"

static int	get_end(const char *str, int start)
{
	int	end;

	end = start;
	while (str[end] && (ft_isalnum(str[end]) || str[end] == '_'))
		end++;
	return (end);
}

static int	process_var_length(const char *str, t_quote_check *var,
				t_env_var *env, t_gc *gc)
{
	int		start;
	int		end;
	char	*var_name;
	char	*var_value;

	if (str[var->i + 1] == '?')
	{
		var->len += 12;
		var->i += 2;
		return (var->len);
	}
	start = var->i + 1;
	end = get_end(str, start);
	if (end > start)
	{
		var_name = extract_var_name(str, start, end, gc);
		if (!var_name)
			return (-1);
		var_value = get_env_value(env, var_name);
		if (var_value)
			var->len += ft_strlen(var_value);
		var->i = end;
	}
	else
	{
		var->len++;
		var->i++;
	}
	return (var->len);
}

int	calculate_max_result_length(const char *str, t_env_var *env, t_gc *gc)
{
	t_quote_check	var;

	var.i = 0;
	var.len = 0;
	while (str[var.i])
	{
		if (str[var.i] == '$')
		{
			var.len = process_var_length(str, &var, env, gc);
			if (var.len < 0)
				return (-1);
		}
		else
		{
			var.len++;
			var.i++;
		}
	}
	return (var.len + 1);
}

char	*process_quotes(const char *str, t_env_var *env, t_exec *exec)
{
	char	*result;
	int		max_len;

	if (!str)
		return (NULL);
	max_len = calculate_max_result_length(str, env, &exec->gc);
	if (max_len < 0)
		return (NULL);
	result = ft_malloc(&exec->gc, max_len);
	if (!result)
		return (NULL);
	process_quoted_text(str, &result, env, exec);
	return (result);
}

void	print_with_quote_handling(const char *str, t_env_var *env, t_exec *exec)
{
	char	*processed;

	if (!str)
		return ;
	processed = process_quotes(str, env, exec);
	if (processed)
		printf("%s", processed);
}
