/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 09:18:08 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exit.h"

void	ft_exit(char **args, int last_exit_status, t_exec *exec)
{
	(void)last_exit_status;
	printf("exit\n");
	if (!args[1])
		cleanup_and_exit(exec, 0);
	if (is_numeric(args[1]))
	{
		if (args[2])
		{
			printf("exit: too many arguments\n");
			exec->exit_status = 1;
			return ;
		}
		cleanup_and_exit(exec, ft_atoi(args[1]) % 256);
	}
	else
	{
		printf("exit: %s: numeric argument required\n", args[1]);
		cleanup_and_exit(exec, 255);
	}
}
