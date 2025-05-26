/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/05/26 22:45:37 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int	g_signal_received;

static void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_signal_received = 0;
}

void signal_fdahouk()
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void function_ignore()
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void function_default()
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void setup_signal_handlers(int sigint_action, int sigquit_action)
{
	if (sigint_action == 1 && sigquit_action == 0)
		signal_fdahouk();
	else if (sigint_action == 1 && sigquit_action == 1)
		function_default();
	else
		function_ignore();
}

void handle_eof_signal(t_exec *exec)
{
	printf("exit\n");
	exec->exit_status = 0;
	cleanup_and_exit(exec, 0); //for the ctrl+d --> exit_code = 0
}

void cleanup_and_exit(t_exec *exec, int exit_code)
{
	int shlvl;
	
	shlvl = get_shell_level(exec);
	if (shlvl <= 1 && exec)
		ft_free_all(&exec->gc);
	exit(exit_code);
}
