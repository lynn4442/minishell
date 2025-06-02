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

int g_signal_received = 0;

int main(int ac, char **av, char **envp)
{
	t_gc		hello;
	t_exec		*exec;
	char		*input;
	char		*copied_input;
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
	update_shlvl(exec);
	setup_interactive_signals();

	while (1)
	{
		g_signal_received = 0;
		input = readline("minihell> ");

		if (!input)
		{
			handle_eof_signal(exec);
			break;
		}
		if (g_signal_received)
		{
			exec->exit_status = g_signal_received;
			free(input);
			continue;
		}
		if (ft_strlen(input) == 0)
		{
			free(input);
			continue;
		}
		add_history(input);
		error_status = 0;
		copied_input = copy_input_string(input, &exec->gc);
		if (!copied_input)
		{
			exec->exit_status = error_status;
			free(input);
			continue;
		}
		cmd_list = parse_command_line(copied_input, exec);
		if (cmd_list)
		{
			setup_parent_signals();
			if (cmd_list->type == PIPE || cmd_list->next)
				execute_with_pipes(exec, cmd_list);
			else
				parse_and_execute(exec, cmd_list);
			setup_interactive_signals();
		}
		free(input);
	}
	ft_free_all(&exec->gc);
	return (exec->exit_status);
}

