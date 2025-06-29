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

static void	shell_loop(t_exec *exec)
{
	char	*input;
	int		input_status;

	while (1)
	{
		input_status = handle_user_input(exec, &input);
		if (input_status == 0)
			break ;
		if (input_status == 1)
			continue ;
		if (is_empty_input(input))
		{
			free(input);
			continue ;
		}
		process_command_line(exec, input);
		free(input);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_gc	hello;
	t_exec	*exec;

	(void)ac;
	(void)av;
	ft_memset(&hello, 0, sizeof(t_gc));
	exec = initialize_shell(&hello, envp);
	if (!exec)
		return (1);
	shell_loop(exec);
	clear_history();
	ft_free_all(&exec->gc);
	return (exec->exit_status);
}
