/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:45:55 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 12:48:28 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static char	*process_output_filename(t_token *current, t_parser *parser)
{
	char	*filename;
	char	*unquoted;

	if (!current->next || current->next->type != TOKEN_WORD)
		return (NULL);
	filename = current->next->value;
	if (filename[0] == '"' && filename[ft_strlen(filename) - 1] == '"')
	{
		unquoted = ft_substr(filename, 1, ft_strlen(filename) - 2);
		if (unquoted)
		{
			filename = ft_strdup(&parser->exec->gc, unquoted);
			free(unquoted);
		}
	}
	return (filename);
}

static void	validate_output_file(char *filename, int is_append)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (is_append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd >= 0)
		close(fd);
}

void	parse_simple_command_output(t_token *current,
		t_parse_simple_cmd *parse, t_parser *parser)
{
	char	*filename;

	parse->last_output_is_append = (current->type == TOKEN_REDIR_APPEND);
	filename = process_output_filename(current, parser);
	if (filename)
	{
		parse->filename = filename;
		validate_output_file(filename, parse->last_output_is_append);
		parse->last_output = filename;
	}
}
