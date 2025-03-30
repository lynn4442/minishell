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

static char **split_preserve_quotes(const char *input, t_gc *gc)
{
	char **result;
	int i = 0;
	int start = 0;
	int count = 0;
	char quote = 0;
	
	// First count the number of arguments
	while (input[i])
	{
		// Skip spaces if we're not in quotes
		while (input[i] && ft_isspace(input[i]) && !quote)
			i++;
		if (!input[i])
			break;
		
		// Mark start of an argument
		start = i;
		
		// Process until end of argument
		while (input[i])
		{
			if (input[i] == '\'' || input[i] == '"')
			{
				if (!quote)
					quote = input[i];
				else if (quote == input[i])
					quote = 0;
			}
			if (ft_isspace(input[i]) && !quote)
				break;
			i++;
		}
		if (start < i)
			count++;
		if (!input[i])
			break;
		i++;
	}

	// Allocate array
	result = ft_malloc(gc, sizeof(char *) * (count + 1));
	if (!result)
		return NULL;

	// Reset for second pass
	i = 0;
	count = 0;
	quote = 0;

	// Second pass: copy arguments
	while (input[i])
	{
		while (input[i] && ft_isspace(input[i]) && !quote)
			i++;
		if (!input[i])
			break;

		start = i;
		while (input[i])
		{
			if (input[i] == '\'' || input[i] == '"')
			{
				if (!quote)
					quote = input[i];
				else if (quote == input[i])
					quote = 0;
			}
			if (ft_isspace(input[i]) && !quote)
				break;
			i++;
		}
		if (start < i)
		{
			result[count] = ft_strndup(gc, input + start, i - start);
			count++;
		}
		if (!input[i])
			break;
		i++;
	}
	result[count] = NULL;
	return result;
}

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
				char ***commands = split_by_pipe(input, exec);
				if (commands && commands[0] && commands[1])
				{
					execute_pipe(exec, commands[0], commands[1], envp);
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
						parse_and_execute(exec, cmd, envp);
					}
				}
			}
		}
		if (ft_strcmp(input, "stop") == 0)
		{
			printf("hello");
			break ;
		}

		// After executing a command, check if it was interrupted
		if (g_signal_received)
			exec->exit_status = g_signal_received;

		free(input);
	}
	ft_free_all(&exec->gc);
	return (exec->exit_status);
}
