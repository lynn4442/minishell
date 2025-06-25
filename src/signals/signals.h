/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/01/19 16:00:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "../../include/minishell.h"

// signal_handling.c
void	handle_sigint(int sig);
void	setup_interactive_signals(void);
void	setup_child_signals(void);
void	setup_parent_signals(void);
void	setup_signal_handlers(int sigint_action, int sigquit_action);

// signal_exit.c
void	handle_eof_signal(t_exec *exec);

#endif 