/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 18:25:53 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/21 01:44:05 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_numeric(const char *str)
{
	int i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void ft_exit(char **args, int last_exit_status)
{
	(void)last_exit_status;
	printf("exit\n");
	if (!args[1])
		exit(1);
	if (is_numeric(args[1]))
	{
		if (args[2])
		{
			printf("exit: too many arguments\n");
			return ;
		}
		exit(ft_atoi(args[1]) % 256);
	}
	else
	{
		printf("exit: %s: numeric argument required\n", args[1]);
		exit(255);
	}
}
