/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:39:22 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/22 15:15:16 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void print_arg(char *arg, t_env_var *env, t_exec *exec)
{
	if (!arg)
		return;

	int i = 0;
	int len = ft_strlen(arg);

	// Handle single quotes - print literally without expansion
	if (len >= 2 && arg[0] == '\'' && arg[len - 1] == '\'')
	{
		write(STDOUT_FILENO, arg + 1, len - 2);
		return;
	}

	// Handle double quotes or unquoted text
	while (i < len)
	{
		// Skip the opening and closing quotes if they exist
		if (i == 0 && arg[0] == '"')
		{
			i++;
			continue;
		}
		if (i == len - 1 && arg[len - 1] == '"')
			break;

		// Handle environment variable expansion
		if (arg[i] == '$' && arg[i + 1])
		{
			// Handle $? special case
			if (arg[i + 1] == '?')
			{
				char *exit_code_str = ft_itoa(exec->exit_status, &exec->gc);
				write(STDOUT_FILENO, exit_code_str, ft_strlen(exit_code_str));
				i += 2;
				continue;
			}

			// Find the end of the variable name
			int start = i + 1;
			int end = start;
			while (arg[end] && (ft_isalnum(arg[end]) || arg[end] == '_'))
				end++;

			// Extract and print the variable value
			if (end > start)
			{
				char *var_name = ft_strndup(&exec->gc, arg + start, end - start);
				char *value = get_env_value(env, var_name);
				if (value)
					write(STDOUT_FILENO, value, ft_strlen(value));
				i = end;
				continue;
			}
		}

		// Print regular character
		write(STDOUT_FILENO, &arg[i], 1);
		i++;
	}
}

void ft_echo(t_cmd_node *cmd, t_env_var *env, t_exec *exec)
{
	int i = 1;
	int no_newline = 0;
	int fd;
	int original_stdout = -1;

	if (!cmd || !cmd->arr || !cmd->arr[0])
		return;

	// Handle output redirection
	fd = handle_output_redirection(cmd);
	if (fd == -1)
	{
		exec->exit_status = 1;
		return;
	}

	if (fd != STDOUT_FILENO)
	{
		original_stdout = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}

	// Check for -n option(s)
	while (cmd->arr[i])
	{
		if (cmd->arr[i][0] == '-')
		{
			int j = 1;
			while (cmd->arr[i][j] == 'n')
				j++;
			if (cmd->arr[i][j] != '\0')
				break;
			no_newline = 1;
			i++;
		}
		else
			break;
	}

	// Print arguments
	int first = 1;
	while (cmd->arr[i])
	{
		if (!first)
			write(STDOUT_FILENO, " ", 1);
		print_arg(cmd->arr[i], env, exec);
		first = 0;
		i++;
	}

	if (!no_newline)
		write(STDOUT_FILENO, "\n", 1);

	// Restore original stdout if needed
	if (original_stdout != -1)
	{
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdout);
	}

	exec->exit_status = 0;
}

