/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:37:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 11:11:53 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int g_signal_received = 0;

/* Helper function to sanitize input by cleaning up excessive redirection symbols */
static char *sanitize_input(char *input, t_gc *gc, int *error_status)
{
	int len = ft_strlen(input);
	char *sanitized = ft_malloc(gc, len + 1);
	int i = 0;
	int j = 0;
	
	(void)error_status; // Mark parameter as intentionally unused
	
	if (!sanitized)
		return input;

	while (input[i])
	{
		sanitized[j++] = input[i++];
	}
	
	sanitized[j] = '\0';
	return sanitized;
}

int main(int ac, char **av, char **envp)
{
	t_gc		hello;
	t_exec		*exec;
	char		*input;
	char		*sanitized_input;
	t_cmd_node	*cmd_list;
	int			error_status;

	(void)ac;
	(void)av;
	memset(&hello, 0, sizeof(t_gc));
	exec = ft_malloc(&hello, sizeof(t_exec));
	if (!exec)
	{
		perror("malloc");
		return (1);
	}
	init_exec(exec);
	init_env(exec, envp);
	update_shlvl(exec);  // Update the SHLVL environment variable
	setup_interactive_signals();  // Setup signal handling

	while (1)
	{
		g_signal_received = 0;  // Reset signal status
		input = readline("minihell> ");
		
		if (!input)  // Handle Ctrl+D (EOF)
		{
			handle_eof_signal(exec);
			break;
		}
		
		// If a signal was received, skip command processing
		if (g_signal_received)
		{
			exec->exit_status = g_signal_received;
			free(input);
			continue;
		}
		
		// Skip empty lines
		if (ft_strlen(input) == 0)
		{
			free(input);
			continue;
		}

		// Add input to history
		add_history(input);
		
		// Initialize error status
		error_status = 0;
		
		// Sanitize input to handle excessive redirection symbols
		sanitized_input = sanitize_input(input, &exec->gc, &error_status);
		
		// If sanitization failed (syntax error), set error status and continue
		if (!sanitized_input)
		{
			exec->exit_status = error_status;
			free(input);
			continue;
		}
		
		// Use the new centralized parser with sanitized input
		cmd_list = parse_command_line(sanitized_input, exec);
		
		// If parsing is successful, execute the commands
		if (cmd_list)
		{
			// Temporarily ignore SIGINT during command execution
			setup_parent_signals();
			
			// Execute commands based on type
			if (cmd_list->type == PIPE || cmd_list->next)
				execute_with_pipes(exec, cmd_list);
			else
				parse_and_execute(exec, cmd_list);
			
			// Restore interactive signals
			setup_interactive_signals();
		}
		
		// Handle an explicit "stop" command
		if (ft_strcmp(input, "stop") == 0)
		{
			printf("hello");
			free(input);
			break;
		}
		
		free(input);
	}
	ft_free_all(&exec->gc);
	return (exec->exit_status);
}
