/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/23 01:09:12 by marvin           ###   ########.fr       */
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

static int	process_var_length(const char *str, int *i,
							int max_len, t_env_var *env)
{
	int		start;
	int		end;
	char	*var_name;
	char	*var_value;

	start = *i + 1;
	end = get_end(str, start);
	if (end > start)
	{
		var_name = extract_var_name(str, start, end);
		if (!var_name)
			return (-1);
		var_value = get_env_value(env, var_name);
		free(var_name);
		if (var_value)
			max_len += ft_strlen(var_value);
		*i = end;
	}
	else
	{
		max_len++;
		(*i)++;
	}
	return (max_len);
}

int	calculate_max_result_length(const char *str, t_env_var *env)
{
	int	i;
	int	max_len;

	i = 0;
	max_len = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			max_len = process_var_length(str, &i, max_len, env);
			if (max_len < 0)
				return (-1);
		}
		else
		{
			max_len++;
			i++;
		}
	}
	return (max_len + 1);
}

char	*process_quotes(const char *str, t_env_var *env, t_exec *exec)
{
	char	*result;
	int		max_len;

	if (!str)
		return (NULL);
	max_len = calculate_max_result_length(str, env);
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
