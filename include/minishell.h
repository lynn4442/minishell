/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:41:12 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/25 16:50:53 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*                               SYSTEM INCLUDES                             */

/* Standard C Library */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdbool.h>
# include <ctype.h>
# include <limits.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <errno.h>
# include <unistd.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <stdio.h>
# include <signal.h>


/* Readline Library */
#ifdef READLINE_MOCK
// Mock readline for Windows development
char *readline(const char *prompt);
void add_history(const char *line);
#else
# include <readline/readline.h>
# include <readline/history.h>
#endif

/*                               PROJECT INCLUDES                            */

# include "../libft/libft.h"
# include "types.h"

//all headers 
# include "../src/env/env.h"
# include "../src/echo/echo.h"
# include "../src/cd/cd.h"
# include "../src/core/core.h"
# include "../src/pwd/pwd.h"
# include "../src/export/export.h"
# include "../src/unset/unset.h"
# include "../src/exit/exit.h"
# include "../src/parser/parser.h"
# include "../src/pipes/pipes.h"
# include "../src/execution/execution.h"
# include "../src/utils/utils.h"
# include "../src/quotes/quotes.h"
# include "../src/redirections/redirections.h"
# include "../src/signals/signals.h"

/*                                EXIT CODES                                 */

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

/*                              GLOBAL VARIABLES                             */

extern int	g_signal_received;

/*                           FUNCTION DECLARATIONS                           */

/* Core Functions */
void		execute_command_supreme(t_exec *exec, t_cmd_node *cmd);
char		*find_command_path(t_exec *exec, const char *cmd);

/* Signal Handling */
void		setup_interactive_signals(void);
void		setup_parent_signals(void);
void		setup_child_signals(void);
void		handle_eof_signal(t_exec *exec);


/* Forward Declarations for Module Functions */
/* (Each module will have its own header with detailed declarations) */

#endif
