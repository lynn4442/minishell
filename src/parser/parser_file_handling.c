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
		ft_putstr_fd(": No such file or directory\n", 2);
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
char	**ft_realloc_append_str_array(char **old, char *new_str, int count, t_gc *gc)
{
	char	**new_arr;
	int		i;

	i = 0;
	new_arr = ft_malloc(gc, sizeof(char *) * (count + 1));
	while (old && old[i])
	{
		new_arr[i] = old[i];
		i++;
	}
	new_arr[i++] = new_str;
	new_arr[i] = NULL;
	return (new_arr);
}
int	parse_simple_command_heredoc(t_token *current, t_parse_simple_cmd *parse, t_parser *parser)
{
	t_token	*next;

	next = current->next;
	if (!next || next->type != TOKEN_WORD)
		return (0);
	parse->heredoc_delimiter = ft_realloc_append_str_array(
		parse->heredoc_delimiter,
		ft_strdup(&parser->exec->gc, next->value),
		++parse->heredoc_count,
		&parser->exec->gc
	);
	parse->last_input = next->value;
	return (1);
}
