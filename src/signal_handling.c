/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/05/22 15:05:14 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int g_signal_received;

//after zabbit l sigaction
static void handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_signal_received = 0;
}

static void setup_signal_handlers(int sigint_action, int sigquit_action)
{
	struct sigaction sa_int;
	struct sigaction sa_quit;

	if (sigint_action == 1)
	{
		sa_int.sa_handler = handle_sigint;
		sa_int.sa_flags = SA_RESTART;
	}
	else if (sigint_action == 0) //ignore
	{
		sa_int.sa_handler = SIG_IGN;
		sa_int.sa_flags = 0;
	}
	else //default
	{
		sa_int.sa_handler = SIG_DFL;
		sa_int.sa_flags = 0;
	}
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
	if (sigquit_action == 0)
		sa_quit.sa_handler = SIG_IGN;
	else
		sa_quit.sa_handler = SIG_DFL;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void setup_interactive_signals(void)
{
	setup_signal_handlers(1, 0);
}

void setup_child_signals(void)
{
	setup_signal_handlers(-1, -1);//defaults for child process
}

void setup_parent_signals(void)
{
	setup_signal_handlers(0, 0);//hone ignore both lal parent
}

void cleanup_and_exit(t_exec *exec, int exit_code)
{
	int shlvl = get_shell_level(exec);//check the shell level

	//if not in nested level
	if (shlvl <= 1 && exec)
		ft_free_all(&exec->gc);
	exit(exit_code);
}

void handle_eof_signal(t_exec *exec)
{
	printf("exit\n");
	exec->exit_status = 0;
	cleanup_and_exit(exec, 0);//for the ctrl+d --> exit_code = 0
}

int get_signal_exit_status(int status)
{
	if (WIFSIGNALED(status))  // Checks if process was terminated by a signal
	{
		int sig = WTERMSIG(status);  // Gets the signal number that terminated the process
		if (sig == SIGINT)
			return 130;
		else if (sig == SIGQUIT)
		{
			printf("Quit (core dumped)\n");
			return 131;//when ctrl+\ inside a infinite loop
		}
		else if (sig == SIGKILL)
			return 137;
		else if (sig == SIGSEGV)
			return 139;
	}
	return WEXITSTATUS(status);  // Gets the exit code from normally terminated process
}
// recheck with fouad this to redo it
