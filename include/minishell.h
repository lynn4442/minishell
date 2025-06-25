/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhussein <hhussein@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:41:12 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/25 21:45:18 by hhussein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
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
# include <readline/readline.h>
# include <readline/history.h>

//all headers 
# include "../libft/libft.h"
# include "types.h"
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

extern int	g_signal_received;

void		execute_command_supreme(t_exec *exec, t_cmd_node *cmd);
char		*find_command_path(t_exec *exec, const char *cmd);

//signal handling
void		setup_interactive_signals(void);
void		setup_parent_signals(void);
void		setup_child_signals(void);
void		handle_eof_signal(t_exec *exec);

#endif