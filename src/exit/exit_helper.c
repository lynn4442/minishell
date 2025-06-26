/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhussein <hhussein@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:47:38 by hhussein          #+#    #+#             */
/*   Updated: 2025/06/25 19:48:28 by hhussein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exit.h"

void	print_numeric_error(const char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

void	print_too_many_args_error(void)
{
	ft_putstr_fd("minishell: exit: too many arguments\n", 2);
}

int	normalize_exit_code(int exit_code)
{
	if (exit_code < 0)
		return (256 + (exit_code % 256));
	return (exit_code % 256);
}

void	handle_no_arguments(t_exec *exec)
{
	ft_free_all(&exec->gc);
	exit(0);
}
