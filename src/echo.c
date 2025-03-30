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
	print_with_quote_handling(arg, env, exec);
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

