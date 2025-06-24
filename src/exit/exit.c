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

static void	print_numeric_error(const char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

static void	print_too_many_args_error(void)
{
	ft_putstr_fd("minishell: exit: too many arguments\n", 2);
}

static int	normalize_exit_code(int exit_code)
{
	if (exit_code < 0)
		return (256 + (exit_code % 256));
	return (exit_code % 256);
}

static void	handle_no_arguments(t_exec *exec)
{
	cleanup_and_exit(exec, 0);
}

static void	handle_non_numeric_argument(const char *arg, t_exec *exec)
{
	print_numeric_error(arg);
	cleanup_and_exit(exec, 2);
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
		cleanup_and_exit(exec, exit_code);
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
