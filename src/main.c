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

int main(int ac, char **av, char **envp)
{
	t_gc		hello;
	t_exec		*exec;
	char		*input;
	char		**args;
	t_cmd_node	*cmd;

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
		
		if (ft_strlen(input) > 0)
		{
			// Check for unclosed quotes first
			if (!check_quotes(input))
			{
				ft_putstr_fd("minishell: syntax error: unclosed quotes\n", 2);
				exec->exit_status = 2;
				free(input);
				continue;
			}

			add_history(input);
			
			// Check if command contains pipe
			if (ft_strchr(input, '|'))
			{
				// Check for leading or trailing pipe
				char *trimmed = ft_strtrim(input, " \t", &exec->gc);
				if (!trimmed || trimmed[0] == '|' || trimmed[ft_strlen(trimmed) - 1] == '|')
				{
					ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
					exec->exit_status = 2;
					free(input);
					continue;
				}

				char ***commands = split_by_pipe(input, exec);
				if (commands && commands[0])
				{
					// Count the number of commands
					int cmd_count = 0;
					while (commands[cmd_count])
						cmd_count++;
					
					if (cmd_count > 1)
					{
						// Temporarily ignore SIGINT during command execution
						setup_parent_signals();
						execute_pipe(exec, commands, cmd_count);
						// Restore interactive signals
						setup_interactive_signals();
					}
					else
					{
						ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
						exec->exit_status = 2;
					}
				}
				else
				{
					ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
					exec->exit_status = 2;
				}
			}
			else
			{
				args = split_preserve_quotes(input, &exec->gc);
				if (args)
				{
					cmd = create_cmd_node(exec, args);
					if (cmd)
					{
						parse_redirections(cmd, cmd->arr);
						process_and_update_args(cmd, cmd->arr);
						// Temporarily ignore SIGINT during command execution
						setup_parent_signals();
						parse_and_execute(exec, cmd);
						// Restore interactive signals
						setup_interactive_signals();
					}
				}
			}
		}
		if (ft_strcmp(input, "stop") == 0)
		{
			printf("hello");
			break;
		}
		free(input);
	}
	
	// Free all allocated memory before exiting
	if (exec)
	{
		ft_free_all(&exec->gc);
	}
	
	return (exec->exit_status);
}
