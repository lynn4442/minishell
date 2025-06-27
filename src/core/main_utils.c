/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 09:27:44 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/27 09:31:18 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

t_exec	*initialize_shell(t_gc *gc, char **envp)
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

void	process_command_line(t_exec *exec, char *input)
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
		return ;
	setup_parent_signals();
	if (cmd_list->type == PIPE || cmd_list->next)
		execute_with_pipes(exec, cmd_list);
	else
		parse_and_execute(exec, cmd_list);
	setup_interactive_signals();
}

int	handle_user_input(t_exec *exec, char **input)
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

int	is_empty_input(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (!ft_isspace(input[i]))
			return (0);
		i++;
	}
	return (1);
}
