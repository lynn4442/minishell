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
	int	exit_code;
	
	(void)last_exit_status;
	printf("exit\n");
	
	if (!args[1])
		cleanup_and_exit(exec, 0);
		
	// Check for multiple arguments
	if (args[2])
	{
		if (is_numeric(args[1]))
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			exec->exit_status = 1;
			return ;
		}
		else
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			cleanup_and_exit(exec, 2);
		}
	}
	
	// Check if argument is numeric
	if (is_numeric(args[1]))
	{
		exit_code = ft_atoi(args[1]);
		// Handle negative numbers and wrap around for values > 255
		if (exit_code < 0)
			exit_code = 256 + (exit_code % 256);
		else
			exit_code = exit_code % 256;
		cleanup_and_exit(exec, exit_code);
	}
	else
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		cleanup_and_exit(exec, 2);
	}
}
