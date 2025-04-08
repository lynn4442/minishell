/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/04/08 09:51:19 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// int g_signal_received = 0;
//zabbit l sigaction
static void handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_signal_received = 130;
}

void setup_interactive_signals(void)
{
	struct sigaction sa_int;
	struct sigaction sa_quit;

	// Prevent readline from handling signals
	rl_catch_signals = 0;

	// Setup SIGINT (Ctrl+C)
	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	//signal(SIGINT, handle_sigint);
	//i can use both but the sigaction is more reliable than signal

	// Setup SIGQUIT (Ctrl+\)
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void setup_child_signals(void)
{
	struct sigaction sa;
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void setup_parent_signals(void)
{
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void handle_eof_signal(t_exec *exec)
{
	printf("exit\n");
	exec->exit_status = 0;
	
	// Check if we're in a nested shell by looking at SHLVL
	int shlvl = 1;  // Default value
	t_env_var *current = exec->env_list;
	while (current)
	{
		if (ft_strcmp(current->key, "SHLVL") == 0)
		{
			shlvl = ft_atoi(current->value);
			break;
		}
		current = current->next;
	}
	
	// Only free memory if we're not in a nested shell
	if (shlvl <= 1)
	{
		// Free the memory before exiting
		if (exec)
		{
			ft_free_all(&exec->gc);
		}
	}
	
	exit(0);  // Always exit with 0 for EOF (Ctrl+D)
}

int get_signal_exit_status(int status)
{
	if (WIFSIGNALED(status))
	{
		int sig = WTERMSIG(status);
		if (sig == SIGINT)
			return 130;
		else if (sig == SIGQUIT)
		{
			printf("Quit (core dumped)\n");
			return 131;
		}
		else if (sig == SIGKILL)
			return 137;
		else if (sig == SIGSEGV)
			return 139;
	}
	return WEXITSTATUS(status);
}
