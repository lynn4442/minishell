/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 18:30:33 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/26 18:30:33 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

void	handle_eof_signal(t_exec *exec)
{
	printf("exit\n");
	exec->exit_status = 0;
	ft_free_all(&exec->gc);
	exit(0);
}
