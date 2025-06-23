/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/23 04:04:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quotes.h"

static int	handle_backslash(const char *str, t_quote_check *st)
{
	if (str[st->i] == '\\' && !st->escaped && st->quote_type != '\'')
	{
		st->escaped = 1;
		if (str[st->i + 1] == '\'' || str[st->i + 1] == '"')
			st->res[st->len++] = str[st->i];
		st->i++;
		return (1);
	}
	return (0);
}

static int	handle_escaped_char(const char *str, t_quote_check *st)
{
	if (st->escaped)
	{
		st->res[st->len++] = str[st->i++];
		st->escaped = 0;
		return (1);
	}
	return (0);
}

int	handle_quote(const char *str, t_quote_check *st)
{
	if (str[st->i] == '\'' || str[st->i] == '"')
	{
		if (st->quote_type == '\0')
		{
			st->quote_type = str[st->i];
			fprintf(stderr, "DEBUG: Start quote '%c' at pos %d\n", str[st->i], st->i);
		}
		else if (str[st->i] == st->quote_type)
		{
			st->quote_type = '\0';
			fprintf(stderr, "DEBUG: End quote '%c' at pos %d\n", str[st->i], st->i);
		}
		else
		{
			st->res[st->len++] = str[st->i];
			fprintf(stderr, "DEBUG: Add quote '%c' to result at pos %d\n", str[st->i], st->i);
		}
		st->i++;
		return (1);
	}
	return (0);
}

static int	handle_quotes_and_escapes(const char *str, t_quote_check *st)
{
	if (handle_backslash(str, st))
		return (1);
	if (handle_escaped_char(str, st))
		return (1);
	if (handle_quote(str, st))
		return (1);
	return (0);
}

int	process_quoted_text(const char *str, char **result,
					t_env_var *env, t_exec *exec)
{
	t_quote_check	st;

	st.i = 0;
	st.len = 0;
	st.res = *result;
	st.quote_type = '\0';
	st.escaped = 0;
	while (str[st.i])
	{
		if (handle_quotes_and_escapes(str, &st))
			continue ;
		if (str[st.i] == '$' && st.quote_type != '\'' && str[st.i + 1])
		{
			st.len = process_special_chars(str, &st, env, exec);
			continue ;
		}
		st.res[st.len++] = str[st.i++];
	}
	st.res[st.len] = '\0';
	return (st.len);
}
