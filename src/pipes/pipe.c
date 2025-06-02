/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhussein <hhussein@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/02 18:39:52 by hhussein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char ***split_by_pipe(char *input, t_exec *exec)
// {
// 	char **parts;
// 	int cmd_count;
// 	char ***commands;
// 	int i;
// 	char	*trimmed;

// 	parts = ft_split(input, '|', &exec->gc);
// 	if (!parts)
// 		return (NULL);
// 	cmd_count = 0;
// 	while (parts[cmd_count])
// 		cmd_count++;
// 	commands = ft_malloc(&exec->gc, (cmd_count + 1) * sizeof(char **));
// 	if (!commands)
// 		return (NULL);
// 	i = 0;
// 	while (parts[i])
// 	{
// 		*trimmed = ft_strtrim(parts[i], " \t", &exec->gc);
// 		if (!trimmed)
// 			return (NULL);
// 		if (ft_strlen(trimmed) == 0)
// 		{
// 			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
// 			exec->exit_status = 2;
// 			return (NULL);
// 		}
// 		commands[i] = split_preserve_quotes(trimmed, &exec->gc);
// 		if (!commands[i])
// 			return (NULL);
// 		i++;
// 	}
// 	commands[i] = (NULL);
// 	return (commands);
// }
static int	count_parts(char **parts)
{
	int count = 0;
	while (parts && parts[count])
		count++;
	return (count);
}
static char	***allocate_commands(int count, t_exec *exec)
{
	char ***commands;

	commands = ft_malloc(&exec->gc, (count + 1) * sizeof(char **));
	return (commands);
}

static char	**process_command_part(char *part, t_exec *exec)
{
	char	*trimmed;
	char	**tokens;

	trimmed = ft_strtrim(part, " \t", &exec->gc);
	if (!trimmed)
		return (NULL);
	if (ft_strlen(trimmed) == 0)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		exec->exit_status = 2;
		return (NULL);
	}
	tokens = split_preserve_quotes(trimmed, &exec->gc);
	return (tokens);
}
char ***split_by_pipe(char *input, t_exec *exec)
{
	char **parts;
	char ***commands;
	int cmd_count;
	int i;

	parts = ft_split(input, '|', &exec->gc);
	if (!parts)
		return (NULL);
	cmd_count = count_parts(parts);
	commands = allocate_commands(cmd_count, exec);
	if (!commands)
		return (NULL);
	i = 0;
	while (i < cmd_count)
	{
		commands[i] = process_command_part(parts[i], exec);
		if (!commands[i])
			return (NULL);
		i++;
	}
	commands[i] = NULL;
	return (commands);
}

static void handle_escape_and_quotes(char c, int *escaped, char *quote)
{
	if (c == '\\' && !(*escaped))
	{
		*escaped = 1;
		return ;
	}
	if (*escaped)
	{
		*escaped = 0;
		return ;
	}
	if (c == '\'' || c == '"')
	{
		if (!(*quote))
			*quote = c;
		else if (*quote == c)
			*quote = 0;
	}
}

// static int count_arguments(const char *input, t_gc *gc)
// {
// 	int i = 0;
// 	int count = 0;
// 	char quote = 0;
// 	int escaped = 0;

// 	(void)gc;
// 	while (input[i])
// 	{
// 		while (input[i] && ft_isspace(input[i]) && !quote)
// 			i++;
// 		if (!input[i])
// 			break;
// 		int start = i;
// 		if (!quote && !escaped && (input[i] == '>' || input[i] == '<'))
// 		{
// 			count++;
// 			if (input[i+1] && input[i] == input[i+1])
// 			{
// 				i += 2;
// 			}
// 			else
// 			{
// 				i++;
// 			}
// 			while (input[i] && ft_isspace(input[i]))
// 				i++;

// 			if (input[i])
// 				continue;
// 			else
// 				break;
// 		}
// 		while (input[i])
// 		{
// 			handle_escape_and_quotes(input[i], &escaped, &quote);
// 			if (escaped)
// 			{
// 				i++;
// 				continue;
// 			}
// 			if (ft_isspace(input[i]) && !quote)
// 				break;
// 			if (!quote && !escaped && (input[i] == '>' || input[i] == '<'))
// 				break;
// 			i++;
// 		}
// 		if (start < i)
// 			count++;
// 		if (!input[i])
// 			break;
// 		if (ft_isspace(input[i]))
// 			i++;
// 	}
// 	return (count);
// }
static void skip_space(const char *input, int *i)
{
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
}
static int is_redirection(char c)
{
	return (c == '<' || c == '>');
}
static int handle_redirection_pipe(const char *input, int *i, int *count)
{
	if (!is_redirection(input[*i]))
		return (0);
	(*count)++;
	if (input[*i + 1] && input[*i] == input[*i + 1])
		*i += 2;
	else
		(*i)++;
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	return (1);
}
static void skip_argument_token(const char *input, int *i, int *escaped, char *quote)
{
	while (input[*i])
	{
		handle_escape_and_quotes(input[*i], escaped, quote);
		if (*escaped)
		{
			(*i)++;
			continue;
		}
		if (!*quote && (ft_isspace(input[*i]) || is_redirection(input[*i])))
			break;
		(*i)++;
	}
}
void	init_r_variables(t_r_variables *vars)
{
	if (!vars)
		return;
	vars->i = 0;
	vars->j = 0;
	vars->k = 0;
	vars->escaped = 0;
	vars->quote = 0;
	vars->count = 0;
	vars->is_echo_command = 0;
}
static int count_arguments(const char *input, t_gc *gc)
{
	t_r_variables var;
	int start;

	init_r_variables(&var);
	(void)gc;
	while (input[var.i])
	{
		skip_space(input, &var.i);
		if (!input[var.i])
			break ;
		start = var.i;
		if (handle_redirection_pipe(input, &var.i, &var.count))
		{
			if (!input[var.i])
				break ;
			continue ;
		}
		skip_argument_token(input, &var.i, &var.escaped, &var.quote);
		if (start < var.i)
			var.count++;
		if (input[var.i] && ft_isspace(input[var.i]))
			var.i++;
	}
	return (var.count);
}
/* Extract a single argument from the input string */
// static char *extract_argument(const char *input, int *i, char *quote, int *escaped, t_gc *gc, int is_echo_command)
// {
// 	int		start;
// 	char	*arg;
// 	int		len;
// 	char	*processed;
// 	int		j;
// 	int		k;
// 	char nested_quote;
// 	int nested_escaped;

// 	while (input[*i] && ft_isspace(input[*i]) && !(*quote))
// 		(*i)++;
// 	if (!input[*i])
// 		return NULL;
// 	if (!(*quote) && !(*escaped) && (input[*i] == '>' || input[*i] == '<'))
// 	{
// 		 start = *i;
// 		if (input[*i + 1] && input[*i] == input[*i + 1])
// 		{
// 			(*i) += 2;
// 			return ft_strndup(gc, input + start, 2);  // Return ">>" or "<<"
// 		}
// 		else
// 		{
// 			(*i)++;
// 			return ft_strndup(gc, input + start, 1);  // Return ">" or "<"
// 		}
// 	}
// 	start = *i;
// 	while (input[*i])
// 	{
// 		handle_escape_and_quotes(input[*i], escaped, quote);
// 		if (*escaped)
// 		{
// 			(*i)++;
// 			continue;
// 		}
// 		if (ft_isspace(input[*i]) && !(*quote))// Break on unquoted space
// 			break ;
// 		if (!(*quote) && !(*escaped) && (input[*i] == '>' || input[*i] == '<'))// Break on unquoted redirection chars
// 			break;
// 		(*i)++;
// 	}
// 	if (start >= *i)
// 		return (NULL);
// 	arg = ft_strndup(gc, input + start, *i - start);// Extract the raw argument
// 	if (!arg)
// 		return NULL;

// 	if (!is_echo_command)// If we're not in 'echo' command and need to process quotes for the result
// 	{
// 		len = ft_strlen(arg);// Handle nested quotes properly
// 		processed = ft_malloc(gc, len + 1);
// 		if (!processed)
// 			return (NULL);
// 		j = 0;  // Input position
// 		k = 0;  // Output position
// 		nested_quote = 0;
// 		nested_escaped = 0;
// 		while (j < len)
// 		{
// 			if ((arg[j] == '\'' || arg[j] == '"') && !nested_escaped)// Skip opening and closing quotes, but preserve their content
// 			{
// 				if (!nested_quote)
// 				{
// 					nested_quote = arg[j];
// 					j++;
// 					continue ;
// 				}
// 				else if (arg[j] == nested_quote)
// 				{
// 					nested_quote = 0;
// 					j++;
// 					continue ;
// 				}
// 			}

// 			if (arg[j] == '\\' && !nested_escaped && (nested_quote == 0 || nested_quote == '"'))// Handle escape sequences
// 			{
// 				nested_escaped = 1;
// 				j++;
// 				continue;
// 			}
// 			if (j < len)// Copy the character
// 			{
// 				processed[k++] = arg[j++];
// 				nested_escaped = 0;
// 			}
// 		}
// 		processed[k] = '\0';
// 		return (processed);
// 	}
// 	return (arg);
// }
static void skip_leading_spaces(const char *input, int *i, char *quote)
{
    while (input[*i] && ft_isspace(input[*i]) && !(*quote))
        (*i)++;
}
static char *extract_redirection(const char *input, int *i, t_gc *gc)
{
    int start = *i;

    if (input[*i + 1] && input[*i] == input[*i + 1])
    {
        *i += 2;
        return ft_strndup(gc, input + start, 2); // ">>" or "<<"
    }
    else
    {
        (*i)++;
        return ft_strndup(gc, input + start, 1); // ">" or "<"
    }
}
static void extract_argument_bounds(const char *input, int *i, char *quote, int *escaped)
{
    while (input[*i])
    {
        handle_escape_and_quotes(input[*i], escaped, quote);
        if (*escaped)
        {
            (*i)++;
            continue;
        }
        if (ft_isspace(input[*i]) && !(*quote))
            break;
        if (!(*quote) && !(*escaped) && (input[*i] == '>' || input[*i] == '<'))
            break;
        (*i)++;
    }
}
void	handle_quote_state(char c, char *nested_quote, int *j)
{
	if ((c == '\'' || c == '"') && *nested_quote == 0)
	{
		*nested_quote = c;
		(*j)++;
	}
	else if (c == *nested_quote)
	{
		*nested_quote = 0;
		(*j)++;
	}
}
int	handle_escape_state(const char *arg, int *j, int *escaped, char quote)
{
	if (arg[*j] == '\\' && *escaped == 0 && (quote == 0 || quote == '"'))
	{
		*escaped = 1;
		(*j)++;
		return (1);
	}
	return (0);
}
char	*process_argument(const char *arg, t_gc *gc)
{
	char	*processed;
	t_r_variables	var;

	init_r_variables(&var);
	processed = ft_malloc(gc, ft_strlen(arg) + 1);
	if (!processed)
		return (NULL);
	while (arg[var.j])
	{
		if ((arg[var.j] == '\'' || arg[var.j] == '"') && var.escaped == 0)
		{
			handle_quote_state(arg[var.j], &var.quote, &var.j);
			continue ;
		}
		if (handle_escape_state(arg, &var.j, &var.escaped, var.quote))
			continue ;
		processed[var.k++] = arg[var.j++];
		var.escaped = 0;
	}
	processed[var.k] = '\0';
	return (processed);
}
static char *extract_argument(const char *input, int *i, char *quote, int *escaped, t_gc *gc, int is_echo_command)
{
    int start;
    char *arg;

    skip_leading_spaces(input, i, quote);
    if (!input[*i])
        return (NULL);
    if (!(*quote) && !(*escaped) && (input[*i] == '>' || input[*i] == '<'))
        return extract_redirection(input, i, gc);
    start = *i;
    extract_argument_bounds(input, i, quote, escaped);
    if (start >= *i)
        return (NULL);
    arg = ft_strndup(gc, input + start, *i - start);
    if (!arg)
        return (NULL);
    if (!is_echo_command)
    {
        char *processed = process_argument(arg, gc);
        return processed ? processed : arg;
    }
    return (arg);
}
char **split_preserve_quotes(const char *input, t_gc *gc)
{
	char **result;
	t_r_variables var;
	int arg_count;

	init_r_variables(&var);
	arg_count = count_arguments(input, gc);
	result = ft_malloc(gc, sizeof(char *) * (arg_count + 1));
	if (!result)
		return (NULL);
	while (var.count < arg_count)
	{
		char *arg = extract_argument(input, &var.i, &var.quote, &var.escaped, gc, var.is_echo_command);
		if (!arg)
			break ;
		if (var.count == 0 && ft_strcmp(arg, "echo") == 0)// Check if this is the echo command
			var.is_echo_command = 1;
		result[var.count++] = arg;
		if (!input[var.i])
			break ;
		var.i++;
	}
	result[var.count] = NULL;
	return (result);
}
