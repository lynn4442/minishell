/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:12:03 by lyoussef          #+#    #+#             */
/*   Updated: 2025/05/26 14:47:10 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args[count])
		count++;
	return (count);
}

void	init_cmd_fields(t_cmd_node *cmd, t_exec *exec)
{
	cmd->in = NULL;
	cmd->out = NULL;
	cmd->type = SMP_CMD;
	cmd->heredoc = NULL;
	cmd->append = 0;
	cmd->err = 0;
	cmd->ex_heredoc = 0; //heyde li zedta
	cmd->exec = exec;
	cmd->next = NULL;
}

int	copy_args(t_exec *exec, t_cmd_node *cmd, char **args, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		cmd->arr[i] = ft_strdup(&exec->gc, args[i]);
		if (!cmd->arr[i])
			return (0);
		i++;
	}
	cmd->arr[count] = NULL;
	return (1);
}

t_cmd_node	*create_cmd_node(t_exec *exec, char **args)
{
	t_cmd_node	*cmd;
	int			count;

	if (!exec || !args)
		return (NULL);
	count = count_args(args);
	cmd = ft_malloc(&exec->gc, sizeof(t_cmd_node));
	if (!cmd)
		return (NULL);
	cmd->arr = ft_malloc(&exec->gc, (count + 1) * sizeof(char *));
	if (!cmd->arr)
		return (NULL);
	init_cmd_fields(cmd, exec);
	if (!copy_args(exec, cmd, args, count))
		return (NULL);
	return (cmd);
}

void	init_exec(t_exec *exec)
{
	if (!exec)
		return ;
	exec->cmd_list = NULL;
	exec->env_list = NULL;
	exec->exit_status = 0;
	exec->gc.head = NULL;
	exec->heredoc_counter = 0;
}
