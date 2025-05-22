/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/05/22 16:21:23 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char ***split_by_pipe(char *input, t_exec *exec)
{
	// Note: This function should be deprecated in favor of the centralized parser
	// It's maintained here for backward compatibility

	char **parts = ft_split(input, '|', &exec->gc);
	if (!parts)
		return NULL;

	// Count number of commands
	int cmd_count = 0;
	while (parts[cmd_count])
		cmd_count++;

	// Allocate array for commands
	char ***commands = ft_malloc(&exec->gc, (cmd_count + 1) * sizeof(char **));
	if (!commands)
		return NULL;

	// Process each command
	int i = 0;
	while (parts[i])
	{
		// Trim whitespace from the command
		char *trimmed = ft_strtrim(parts[i], " \t", &exec->gc);
		if (!trimmed)
			return NULL;

		// Check for empty commands
		if (ft_strlen(trimmed) == 0)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
			exec->exit_status = 2;
			return NULL;
		}

		// Use split_preserve_quotes to handle quoted arguments and split on spaces
		commands[i] = split_preserve_quotes(trimmed, &exec->gc);
		if (!commands[i])
			return NULL;

		i++;
	}
	commands[i] = NULL;

	return commands;
}

static void handle_escape_and_quotes(char c, int *escaped, char *quote)
{
	if (c == '\\' && !(*escaped))
	{
		*escaped = 1;
		return;
	}

	if (*escaped)
	{
		*escaped = 0;
		return;
	}
	if (c == '\'' || c == '"')
	{
		if (!(*quote))
		{
			*quote = c;
		}
		else if (*quote == c)
		{
			*quote = 0;
		}
		// Otherwise, it's a quote of a different type while inside a quote
		// For example: a double quote inside single quotes, ignore it
	}
}

static int count_arguments(const char *input, t_gc *gc)
{
	int i = 0;
	int count = 0;
	char quote = 0;
	int escaped = 0;

	(void)gc;

	while (input[i])
	{
		while (input[i] && ft_isspace(input[i]) && !quote)
			i++;
		if (!input[i])
			break;
		int start = i;
		if (!quote && !escaped && (input[i] == '>' || input[i] == '<'))
		{
			count++;
			if (input[i+1] && input[i] == input[i+1])
			{
				i += 2;
			}
			else
			{
				i++;
			}
			while (input[i] && ft_isspace(input[i]))
				i++;

			if (input[i])
				continue;
			else
				break;
		}
		while (input[i])
		{
			handle_escape_and_quotes(input[i], &escaped, &quote);
			if (escaped)
			{
				i++;
				continue;
			}
			if (ft_isspace(input[i]) && !quote)
				break;
			if (!quote && !escaped && (input[i] == '>' || input[i] == '<'))
				break;
			i++;
		}

		if (start < i)
			count++;

		if (!input[i])
			break;

		// Important! Increment i to avoid infinite loop if not already at end
		if (ft_isspace(input[i]))
			i++;
	}

	return count;
}

/* Extract a single argument from the input string */
static char *extract_argument(const char *input, int *i, char *quote, int *escaped, t_gc *gc, int is_echo_command)
{
	// Skip spaces if not in quotes
	while (input[*i] && ft_isspace(input[*i]) && !(*quote))
		(*i)++;
	if (!input[*i])
		return NULL;

	// Check for redirection operators
	if (!(*quote) && !(*escaped) && (input[*i] == '>' || input[*i] == '<'))
	{
		int start = *i;

		// Handle >> or << as a single token
		if (input[*i + 1] && input[*i] == input[*i + 1])
		{
			(*i) += 2;
			return ft_strndup(gc, input + start, 2);  // Return ">>" or "<<"
		}
		else
		{
			(*i)++;
			return ft_strndup(gc, input + start, 1);  // Return ">" or "<"
		}
	}

	int start = *i;

	// Process until end of argument
	while (input[*i])
	{
		handle_escape_and_quotes(input[*i], escaped, quote);

		if (*escaped)
		{
			(*i)++;
			continue;
		}

		// Break on unquoted space
		if (ft_isspace(input[*i]) && !(*quote))
			break;

		// Break on unquoted redirection chars
		if (!(*quote) && !(*escaped) && (input[*i] == '>' || input[*i] == '<'))
			break;

		(*i)++;
	}

	if (start >= *i)
		return NULL;

	// Extract the raw argument
	char *arg = ft_strndup(gc, input + start, *i - start);
	if (!arg)
		return NULL;

	// If we're not in 'echo' command and need to process quotes for the result
	if (!is_echo_command)
	{
		// Handle nested quotes properly
		int len = ft_strlen(arg);
		char *processed = ft_malloc(gc, len + 1);
		if (!processed)
			return NULL;

		int j = 0;  // Input position
		int k = 0;  // Output position
		char nested_quote = 0;
		int nested_escaped = 0;

		while (j < len)
		{
			// Skip opening and closing quotes, but preserve their content
			if ((arg[j] == '\'' || arg[j] == '"') && !nested_escaped)
			{
				if (!nested_quote)
				{
					nested_quote = arg[j];
					j++;
					continue;
				}
				else if (arg[j] == nested_quote)
				{
					nested_quote = 0;
					j++;
					continue;
				}
			}

			// Handle escape sequences
			if (arg[j] == '\\' && !nested_escaped && (nested_quote == 0 || nested_quote == '"'))
			{
				nested_escaped = 1;
				j++;
				continue;
			}

			// Copy the character
			if (j < len)
			{
				processed[k++] = arg[j++];
				nested_escaped = 0;
			}
		}
		processed[k] = '\0';

		return processed;
	}

	return arg;
}

char **split_preserve_quotes(const char *input, t_gc *gc)
{
	// Note: This function should be used by the parser,
	// but the direct calls from outside should go through the centralized parser

	char **result;
	int i = 0;
	int count = 0;
	char quote = 0;
	int escaped = 0;
	int is_echo_command = 0;

	// Count the number of arguments
	int arg_count = count_arguments(input, gc);

	// Allocate array
	result = ft_malloc(gc, sizeof(char *) * (arg_count + 1));
	if (!result)
		return NULL;

	// Scan for arguments
	i = 0;
	while (count < arg_count)
	{
		char *arg = extract_argument(input, &i, &quote, &escaped, gc, is_echo_command);
		if (!arg)
			break;

		// Check if this is the echo command
		if (count == 0 && ft_strcmp(arg, "echo") == 0)
			is_echo_command = 1;

		result[count++] = arg;

		if (!input[i])
			break;
		i++;
	}

	result[count] = NULL;
	return result;
}
