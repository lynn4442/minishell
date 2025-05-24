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

static int	handle_exit_status(char *res, int len, t_exec *exec)
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

static int	handle_variable(const char *str, int *i, char *res,
					int len, t_env_var *env, t_exec *exec)
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
		var_name = ft_strndup(&exec->gc, str + start, end - start);
		var_value = get_env_value(env, var_name);
		if (var_value)
		{
			while (*var_value)
				res[len++] = *var_value++;
		}
		*i = end;
	}
	else
	{
		res[len++] = str[(*i)++];
	}
	return (len);
}

static int	process_special_chars(const char *str, int *i, char *res,
					int len, t_env_var *env, t_exec *exec)
{
	if (str[*i + 1] == '?')
	{
		len = handle_exit_status(res, len, exec);
		*i += 2;
	}
	else
	{
		len = handle_variable(str, i, res, len, env, exec);
	}
	return (len);
}

static int	process_quoted_text(const char *str, char **result,
					t_env_var *env, t_exec *exec)
{
	int		i;
	int		len;
	char	*res;
	char	quote_type;
	int		escaped;

	i = 0;
	len = 0;
	res = *result;
	quote_type = '\0';
	escaped = 0;

	while (str[i])
	{
		// Handle escape character (outside quotes or in double quotes)
		if (str[i] == '\\' && !escaped && quote_type != '\'')
		{
			escaped = 1;
			// Keep the backslash for escaped quotes
			if (str[i + 1] == '\'' || str[i + 1] == '"')
				res[len++] = str[i];
			i++;
			continue;
		}

		// Handle escaped character
		if (escaped)
		{
			res[len++] = str[i++];
			escaped = 0;
			continue;
		}

		// Handle quotes
		if ((str[i] == '\'' || str[i] == '"'))
		{
			if (quote_type == '\0')
				quote_type = str[i]; // Start quoted section
			else if (str[i] == quote_type)
				quote_type = '\0';   // End quoted section
			else
				res[len++] = str[i]; // Different quote inside quoted section

			i++;
			continue;
		}

		// Handle variable expansion (outside quotes or in double quotes)
		if (str[i] == '$' && quote_type != '\'' && str[i + 1])
		{
			len = process_special_chars(str, &i, res, len, env, exec);
			continue;
		}

		// Copy regular character
		res[len++] = str[i++];
	}

	res[len] = '\0';
	return (len);
}

static char	*extract_var_name(const char *str, int start, int end)
{
	char	*var_name;

	var_name = malloc(end - start + 1);
	if (!var_name)
		return (NULL);
	ft_strncpy(var_name, str + start, end - start);
	var_name[end - start] = '\0';
	return (var_name);
}

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
			// Skip escaped character
			i++;
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
