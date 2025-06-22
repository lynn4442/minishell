/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Heredoc_quotes_handling.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 19:16:31 by hhussein          #+#    #+#             */
/*   Updated: 2025/06/23 01:30:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quotes.h"

static void	init_heredoc_state(t_quote_check *st, char *result)
{
	st->i = 0;
	st->len = 0;
	st->res = result;
	st->quote_type = '\0';
}

static int	handle_heredoc_exit_status(t_quote_check *st, t_exec *exec)
{
	char	*exit_status;
	int		j;

	exit_status = ft_itoa(exec->exit_status, &exec->gc);
	j = 0;
	while (exit_status && exit_status[j])
		st->res[st->len++] = exit_status[j++];
	st->i += 2;
	return (1);
}

static int	handle_heredoc_variable(const char *str, t_quote_check *st,
									t_env_var *env, t_exec *exec)
{
	int		start;
	int		end;
	char	*var_name;
	char	*var_value;

	start = st->i + 1;
	end = start;
	while (ft_isalnum(str[end]) || str[end] == '_')
		end++;
	var_name = ft_strndup(&exec->gc, str + start, end - start);
	var_value = get_env_value(env, var_name);
	while (var_value && *var_value)
		st->res[st->len++] = *var_value++;
	st->i = end;
	return (1);
}

static int	handle_heredoc_dollar(const char *str, t_quote_check *st,
									t_env_var *env, t_exec *exec)
{
	if (str[st->i + 1] == '?')
		return (handle_heredoc_exit_status(st, exec));
	if (ft_isalpha(str[st->i + 1]) || str[st->i + 1] == '_')
		return (handle_heredoc_variable(str, st, env, exec));
	return (0);
}

char	*process_heredoc_quotes(const char *str, t_env_var *env, t_exec *exec)
{
	t_quote_check	st;
	char			*result;
	int				max_len;

	max_len = calculate_max_result_length(str, env);
	if (max_len < 0)
		return (NULL);
	result = ft_malloc(&exec->gc, max_len);
	if (!result)
		return (NULL);
	init_heredoc_state(&st, result);
	while (str[st.i])
	{
		if (str[st.i] == '\'' || str[st.i] == '"')
		{
			st.res[st.len++] = str[st.i++];
			continue ;
		}
		if (str[st.i] == '$' && handle_heredoc_dollar(str, &st, env, exec))
			continue ;
		st.res[st.len++] = str[st.i++];
	}
	st.res[st.len] = '\0';
	return (st.res);
}
