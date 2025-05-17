/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_core.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 00:17:43 by marvin            #+#    #+#             */
/*   Updated: 2025/05/18 00:17:43 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_echo(t_cmd_node *cmd, t_env_var *env, t_exec *exec)
{
	int	i;
	int	no_newline;
	int	original_stdout;

	i = 1;
	original_stdout = -1;
	if (!cmd || !cmd->arr || !cmd->arr[0])
	{
		ft_putstr_fd("minishell: echo: invalid arguments\n", 2);
		exec->exit_status = 1;
		return ;
	}
	if (handle_echo_redirection(cmd, &original_stdout, exec) == -1)
		return ;
	no_newline = parse_echo_options(cmd->arr, &i);
	print_echo_args(cmd->arr, i, env, exec);
	if (!no_newline)
		printf("\n");
	restore_echo_output(original_stdout, exec);
	exec->exit_status = 0;
}
