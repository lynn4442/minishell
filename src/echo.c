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
	if (arg[0] == '$' && arg[1] != '\0')
	{
		if (arg[1] == '?')
		{
			char *exit_code_str = ft_itoa(exec->exit_status, &exec->gc);
			write(STDOUT_FILENO, exit_code_str, ft_strlen(exit_code_str));
		}
		else
		{
			char *env_value = get_env_value(env, arg + 1);
			if (env_value)
				write(STDOUT_FILENO, env_value, ft_strlen(env_value));
		}
	}
	else
	{
		// Strip quotes if present
		int len = ft_strlen(arg);
		if (len >= 2 && ((arg[0] == '"' && arg[len - 1] == '"') ||
						(arg[0] == '\'' && arg[len - 1] == '\'')))
		{
			write(STDOUT_FILENO, arg + 1, len - 2);
		}
		else
		{
			write(STDOUT_FILENO, arg, len);
		}
	}
}

void ft_echo(t_cmd_node *cmd, t_env_var *env, t_exec *exec)
{
	int i = 1;
	int no_newline = 0;
	int original_stdout = -1;

	if (!cmd || !cmd->arr || !cmd->arr[0])
		return;

	// Handle output redirection first
	if (cmd->out)
	{
		int fd;
		if (cmd->append)
			fd = open(cmd->out, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			
		if (fd == -1)
		{
			perror("minishell");
			exec->exit_status = 1;
			return;
		}
		original_stdout = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}

	// Check for -n option(s)
	while (cmd->arr[i])
	{
		if (ft_strcmp(cmd->arr[i], "-n") == 0)
		{
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

