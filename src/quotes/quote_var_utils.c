/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_var_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 18:27:59 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/26 18:27:59 by lyoussef         ###   ########.fr       */
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

static int	handle_question_mark(t_quote_check *var)
{
	var->len += 12;
	var->i += 2;
	return (var->len);
}

static int	no_valid_var(t_quote_check *var)
{
	var->len++;
	var->i++;
	return (var->len);
}

static int	process_var_length(const char *str, t_quote_check *var,
				t_env_var *env, t_gc *gc)
{
	int		start;
	int		end;
	char	*var_name;
	char	*value;

	if (str[var->i + 1] == '?')
		return (handle_question_mark(var));
	start = var->i + 1;
	end = get_end(str, start);
	if (end <= start)
		return (no_valid_var(var));
	var_name = extract_var_name(str, start, end, gc);
	if (!var_name)
		return (-1);
	value = get_env_value(env, var_name);
	if (value)
		var->len += ft_strlen(value);
	var->i = end;
	return (var->len);
}

int	calculate_max_result_length(const char *str,
			t_env_var *env, t_gc *gc)
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
