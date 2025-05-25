/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 22:33:01 by marvin            #+#    #+#             */
/*   Updated: 2025/05/24 22:33:01 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_HANDLING_H
# define SIGNAL_HANDLING_H

# include "../minishell.h"

void	handle_sigint(int sig);
void	handle_eof_signal(t_exec *exec);

void	setup_signals(int interactive);
void	setup_interactive_signals(void);
void	setup_child_signals(void);
void	setup_parent_signals(void);

int		get_signal_exit_status(int status);

#endif