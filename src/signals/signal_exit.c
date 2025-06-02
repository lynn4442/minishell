/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/05/26 23:13:07 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"
#include "../../minishell.h"

void	handle_eof_signal(t_exec *exec)
{
	printf("exit\n");
	exec->exit_status = 0;
	cleanup_and_exit(exec, 0);
}

void	cleanup_and_exit(t_exec *exec, int exit_code)
{
	int	shlvl;

	shlvl = get_shell_level(exec);
	if (shlvl <= 1 && exec)
		ft_free_all(&exec->gc);
	exit(exit_code);
}
