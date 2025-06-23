/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 09:18:08 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

int	g_signal_received = 0;

/* Initialize execution environment */
static t_exec	*initialize_shell(t_gc *gc, char **envp)
{
	t_exec	*exec;

	exec = ft_malloc(gc, sizeof(t_exec));
	if (!exec)
	{
		perror("malloc");
		return (NULL);
	}
	init_exec(exec);
	init_env(exec, envp);
	update_shlvl(exec);
	setup_interactive_signals();
	return (exec);
}

/* Process and execute a single command line */
static void	process_command_line(t_exec *exec, char *input)
{
	char		*copied_input;
	t_cmd_node	*cmd_list;

	copied_input = copy_input_string(input, &exec->gc);
	if (!copied_input)
	{
		exec->exit_status = 1;
		return ;
	}
	
	cmd_list = parse_command_line(copied_input, exec);
	if (!cmd_list)
	{
		// If parsing failed, the error message was already printed
		// and exit_status was set in the parser
		return ;
	}
	
	// Only execute if parsing was successful
	setup_parent_signals();
	if (cmd_list->type == PIPE || cmd_list->next)
		execute_with_pipes(exec, cmd_list);
	else
		parse_and_execute(exec, cmd_list);
	setup_interactive_signals();
}

/* Handle user input and basic validation */
static int	handle_user_input(t_exec *exec, char **input)
{
	*input = readline("minihell> ");
	if (!*input)
	{
		handle_eof_signal(exec);
		return (0);
	}
	if (g_signal_received)
	{
		exec->exit_status = g_signal_received;
		free(*input);
		return (1);
	}
	if (ft_strlen(*input) == 0)
	{
		free(*input);
		return (1);
	}
	add_history(*input);
	return (2);
}

int	main(int ac, char **av, char **envp)
{
	t_gc	hello;
	t_exec	*exec;
	char	*input;
	int		input_status;

	(void)ac;
	(void)av;
	memset(&hello, 0, sizeof(t_gc));
	exec = initialize_shell(&hello, envp);
	if (!exec)
		return (1);
	while (1)
	{
		g_signal_received = 0;
		input_status = handle_user_input(exec, &input);
		if (input_status == 0)
			break ;
		if (input_status == 1)
			continue ;
		process_command_line(exec, input);
		free(input);
	}
	ft_free_all(&exec->gc);
	return (exec->exit_status);
}
