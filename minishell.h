/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:41:12 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/02 10:37:06 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <ctype.h>
# include <unistd.h>
# include <limits.h>
# include <string.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <stdbool.h>
# include "libft/libft.h"
# include <signal.h>
# include "src/include/types.h"
# include "src/parser/parser.h"
# include "src/env/env.h"
# include "src/echo/echo.h"
# include "src/unset/unset.h"
# include "src/cd/cd.h"
# include "src/export/export.h"
# include "src/quotes/quotes.h"
# include "src/redirections/redirections.h"
# include "src/pipes/pipes.h"
# include "src/execution/execution.h"
# include "src/pwd/pwd.h"
# include "src/exit/exit.h"
# include "src/core/core.h"
# include "src/utils/utils.h"

//0	Success (No error)
//1	General error (Miscellaneous error)
//2	Incorrect usage (Misuse of shell builtins)
//126	Command found but not executable
//127	Command not found
//128	Invalid argument to exit
//130	Command terminated by Ctrl+C (SIGINT)
//137	Command killed with kill -9 (SIGKILL)
//139	Segmentation fault (SIGSEGV)
//255	Exit status out of range

extern int	g_signal_received;

// Core execution functions used across modules
void		execute_command_supreme(t_exec *exec, t_cmd_node *cmd);
char		*find_command_path(t_exec *exec, const char *cmd);

// Signal handling functions used across modules
void		setup_interactive_signals(void);
void		setup_parent_signals(void);
void		setup_child_signals(void);
void		cleanup_and_exit(t_exec *exec, int exit_code);
void		handle_eof_signal(t_exec *exec);
int			get_signal_exit_status(int status);

#endif
