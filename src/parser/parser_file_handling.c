/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_file_handling.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wasmar <wasmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 00:00:00 by wasmar            #+#    #+#             */
/*   Updated: 2025/01/11 00:00:00 by wasmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	parse_simple_command_output(t_token *current,
		t_parse_simple_cmd *parse, t_parser *parser)
{
	char	*unquoted;

	unquoted = NULL;
	parse->last_output_is_append = (current->type == TOKEN_REDIR_APPEND);
	if (current->next && current->next->type == TOKEN_WORD)
	{
		parse->filename = current->next->value;
		if (parse->filename[0] == '"'
			&& parse->filename[ft_strlen(parse->filename) - 1] == '"')
		{
			unquoted = ft_substr(parse->filename, 1, ft_strlen(parse->filename)
					- 2);
			if (unquoted)
			{
				parse->filename = ft_strdup(&parser->exec->gc, unquoted);
				free(unquoted);
			}
		}
		parse->last_output = parse->filename;
	}
}

char	*process_input_filename(t_token *current, t_parser *parser)
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

int	validate_input_file(char *filename, t_parse_simple_cmd *parse)
{
	if (access(filename, F_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": No such file or directory 4\n", 2);
		parse->sflag = 1;
		return (1);
	}
	return (0);
}

int	parse_simple_command_input(t_token *current, t_parse_simple_cmd *parse,
		t_parser *parser)
{
	char	*filename;

	filename = process_input_filename(current, parser);
	if (!filename)
		return (0);
	parse->last_input = filename;
	if (validate_input_file(filename, parse))
	{
		if (current && current->next)
			parser->current_token = current->next;
		return (1);
	}
	return (0);
}
char	**ft_realloc_append_str_array(char **old, char *new_str, t_gc *gc)
{
	char	**new_arr;
	// char	*dequoted;
	int		i;
	int		j;

	i = 0;
	while (old && old[i])
		i++;
	new_arr = ft_malloc(gc, sizeof(char *) * (i + 2));
	if (!new_arr)
		return (NULL);
	j = 0;
	while (j < i)
	{
		new_arr[j] = old[j];
		j++;
	}
	new_arr[j++] = new_str;
	new_arr[j] = NULL;
	return (new_arr);
}
int	parse_simple_command_heredoc(t_token *current, t_parse_simple_cmd *parse, t_parser *parser)
{
	t_token	*next;
	char	*raw;
	char	*cleaned;
	size_t	len;

	next = current->next;
	if (!next || next->type != TOKEN_WORD)
		return (0);
	raw = next->value;
	len = ft_strlen(raw);
	if ((raw[0] == '"' && raw[len - 1] == '"') ||
		(raw[0] == '\'' && raw[len - 1] == '\''))
		cleaned = ft_substr(raw, 1, len - 2);
	else
		cleaned = ft_strdup(&parser->exec->gc, raw);
	parse->heredoc_delimiter = ft_realloc_append_str_array(
		parse->heredoc_delimiter,
		cleaned,
		&parser->exec->gc);
	parse->last_input = next->value;
	return (1);
}
