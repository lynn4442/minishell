/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_quotes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:45:55 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 12:48:28 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*process_quoted_token(char *val, t_parser *parser)
{
	char	*unquoted;
	char	*result;
	int		len;

	len = ft_strlen(val);
	if ((val[0] == '"' && val[len - 1] == '"')
		|| (val[0] == '\'' && val[len - 1] == '\''))
	{
		unquoted = ft_substr(val, 1, len - 2);
		result = ft_strdup(&parser->exec->gc, unquoted);
		free(unquoted);
		return (result);
	}
	return (ft_strdup(&parser->exec->gc, val));
}
