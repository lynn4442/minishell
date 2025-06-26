/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:17:08 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/26 16:17:08 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "echo.h"

void	print_single_arg(char *arg, t_env_var *env, t_exec *exec)
{
	(void)env;
	(void)exec;
	if (!arg)
		return ;
	printf("%s", arg);
}

int	parse_echo_options(char **args, int *i)
{
	int	no_newline;
	int	j;
	int	valid_option;

	no_newline = 0;
	while (args[*i] && args[*i][0] == '-')
	{
		j = 1;
		valid_option = 1;
		while (args[*i][j])
		{
			if (args[*i][j] != 'n')
			{
				valid_option = 0;
				break ;
			}
			j++;
		}
		if (!valid_option || j == 1)
			break ;
		no_newline = 1;
		(*i)++;
	}
	return (no_newline);
}

void	print_echo_args(char **args, int i, t_env_var *env, t_exec *exec)
{
	int	first_arg;

	first_arg = 1;
	while (args[i])
	{
		if (!first_arg)
			printf(" ");
		print_single_arg(args[i], env, exec);
		first_arg = 0;
		i++;
	}
}
