/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/05/23 22:58:16 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	process_var_length(const char *str, int *i, int max_len, t_env_var *env)
{
	int		start;
	int		end;
	char	*var_name;
	char	*var_value;

	start = *i + 1;
	end = start;
	while (str[end] && (ft_isalnum(str[end]) || str[end] == '_'))
		end++;
	if (end > start)
	{
		var_name = extract_var_name(str, start, end);
		if (!var_name)
			return (-1);
		var_value = get_env_value(env, var_name);
		free(var_name);
		max_len += var_value ? strlen(var_value) : 0;
		*i = end;
	}
	else
	{
		max_len++;
		(*i)++;
	}
	return (max_len);
}

static int	calculate_max_result_length(const char *str, t_env_var *env)
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
	return (max_len + 1); // +1 for '\0'
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
			i++; 	// Skip escaped character
			if (str[i])
				i++;
			continue;
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
void	print_with_quote_handling(const char *str, t_env_var *env, t_exec *exec)
{
	char	*processed;

	if (!str)
		return;
	processed = process_quotes(str, env, exec);
	if (processed)
		printf("%s", processed);
}
