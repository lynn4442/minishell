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

static void	handle_non_numeric_argument(const char *arg, t_exec *exec)
{
	print_numeric_error(arg);
	ft_free_all(&exec->gc);
	exit(2);
}

static void	handle_too_many_arguments(const char *first_arg, t_exec *exec)
{
	if (is_numeric(first_arg))
	{
		print_too_many_args_error();
		exec->exit_status = 1;
	}
	else
		handle_non_numeric_argument(first_arg, exec);
}

static void	handle_single_argument(const char *arg, t_exec *exec)
{
	int	exit_code;

	if (is_numeric(arg))
	{
		exit_code = ft_atoi(arg);
		exit_code = normalize_exit_code(exit_code);
		ft_free_all(&exec->gc);
		exit(exit_code);
	}
	else
		handle_non_numeric_argument(arg, exec);
}

void	ft_exit(char **args, int last_exit_status, t_exec *exec)
{
	(void)last_exit_status;
	printf("exit\n");
	if (!args[1])
		handle_no_arguments(exec);
	else if (args[2])
		handle_too_many_arguments(args[1], exec);
	else
		handle_single_argument(args[1], exec);
}
