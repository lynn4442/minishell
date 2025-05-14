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

void print_single_arg(char *arg, t_env_var *env, t_exec *exec)
{
	if (!arg)
		return;
	
	char *processed = process_quotes(arg, env, exec);
	if (processed)
		printf("%s", processed);
}

/* Echo command implementation */
void ft_echo(t_cmd_node *cmd, t_env_var *env, t_exec *exec)
{
	int i = 1;
	int no_newline = 0;
	int fd;
	int original_stdout = -1;
	int first_arg = 1;

	if (!cmd || !cmd->arr || !cmd->arr[0])
	{
		ft_putstr_fd("minishell: echo: invalid arguments\n", 2);
		exec->exit_status = 1;
		return;
	}

	// Handle output redirection
	fd = handle_output_redirection(cmd);
	if (fd == -1)
	{
		exec->exit_status = 1;
		return;
	}
	
	// Setup redirection if needed
	if (fd != STDOUT_FILENO)
	{
		original_stdout = dup(STDOUT_FILENO);
		if (original_stdout == -1 || dup2(fd, STDOUT_FILENO) == -1)
		{
			ft_putstr_fd("minishell: echo: redirection error\n", 2);
			if (original_stdout != -1)
				close(original_stdout);
			close(fd);
			exec->exit_status = 1;
			return;
		}
		close(fd);
	}

	// Check for -n option
	while (cmd->arr[i] && cmd->arr[i][0] == '-')
	{
		int j = 1;
		int valid_option = 1;
		
		// Check if it's a valid -n option (all 'n' characters)
		while (cmd->arr[i][j])
		{
			if (cmd->arr[i][j] != 'n')
			{
				valid_option = 0;
				break;
			}
			j++;
		}
		
		if (!valid_option || j == 1) // Not a valid -n option or just "-"
			break;
			
		no_newline = 1;
		i++;
	}

	// Print arguments
	while (cmd->arr[i])
	{
		if (!first_arg)
			printf(" ");
		print_single_arg(cmd->arr[i], env, exec);
		first_arg = 0;
		i++;
	}

	// Add newline if needed
	if (!no_newline)
		printf("\n");

	// Restore stdout if needed
	if (original_stdout != -1)
	{
		if (dup2(original_stdout, STDOUT_FILENO) == -1)
		{
			ft_putstr_fd("minishell: echo: failed to restore stdout\n", 2);
			exec->exit_status = 1;
		}
		close(original_stdout);
	}

	exec->exit_status = 0;
}

