/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 20:59:04 by hhussein          #+#    #+#             */
/*   Updated: 2025/06/25 23:32:16 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipes.h"

void	print_parent_debug(t_exec *exec, int i, int cmd_count)
{
	ft_putstr_fd("minishell: setting up command ", 2);
	ft_putstr_fd(ft_itoa(i + 1, &exec->gc), 2);
	ft_putstr_fd("/", 2);
	ft_putstr_fd(ft_itoa(cmd_count, &exec->gc), 2);
	ft_putstr_fd("\n", 2);
}
void	print_child_debug(t_cmd_node *current)
{
	if (current->arr && current->arr[0])
	{
		ft_putstr_fd("minishell: child process for command: ", 2);
		ft_putstr_fd(current->arr[0], 2);
		ft_putstr_fd("\n", 2);
	}
}
void	debug_print_cmd(t_exec *exec, t_cmd_node *cmd, int index)
{
	int	j;

	ft_putstr_fd("minishell: cmd", 2);
	ft_putstr_fd(ft_itoa(index, &exec->gc), 2);
	ft_putstr_fd(": ", 2);
	j = 0;
	while (cmd->arr && cmd->arr[j])
	{
		ft_putstr_fd(cmd->arr[j], 2);
		ft_putstr_fd(" ", 2);
		j++;
	}
	if (cmd->out)
	{
		if (cmd->append)
			ft_putstr_fd(">> ", 2);
		else
			ft_putstr_fd("> ", 2);
		ft_putstr_fd(cmd->out, 2);
		ft_putstr_fd(" ", 2);
	}
	if (cmd->in)
	{
		ft_putstr_fd("< ", 2);
		ft_putstr_fd(cmd->in, 2);
		ft_putstr_fd(" ", 2);
	}
	ft_putstr_fd("\n", 2);
}
void	debug_pipeline_commands(t_exec *exec,
	t_cmd_node *cmd_list, int cmd_count)
{
	t_cmd_node	*current;
	int			index;

	ft_putstr_fd("minishell: setting up pipeline with ", 2);
	ft_putstr_fd(ft_itoa(cmd_count, &exec->gc), 2);
	ft_putstr_fd(" commands\n", 2);
	current = cmd_list;
	index = 0;
	while (current)
	{
		debug_print_cmd(exec, current, index);
		current = current->next;
		index++;
	}
}
void	print_wait_debug(t_exec *exec, int i, int status)
{
	ft_putstr_fd("minishell: command ", 2);
	ft_putstr_fd(ft_itoa(i + 1, &exec->gc), 2);
	ft_putstr_fd(" finished with status ", 2);
	ft_putstr_fd(ft_itoa(WEXITSTATUS(status), &exec->gc), 2);
	ft_putstr_fd("\n", 2);
}
void	print_redirection_debug_info(t_cmd_node *cmd)
{
	ft_putstr_fd("minishell: with input redirection from: ", 2);
	ft_putstr_fd(cmd->in, 2);
	ft_putstr_fd("\n", 2);
	ft_putstr_fd("minishell: with output redirection to: ", 2);
	ft_putstr_fd(cmd->out, 2);
	if (cmd->append)
		ft_putstr_fd(" (append)", 2);
	else
		ft_putstr_fd(" (truncate)", 2);
	ft_putstr_fd("\n", 2);
}

void	print_command_debug_info(t_cmd_node *cmd)
{
	int	i;

	ft_putstr_fd("minishell: executing command: ", 2);
	i = 0;
	while (cmd->arr && cmd->arr[i])
	{
		ft_putstr_fd(cmd->arr[i], 2);
		ft_putstr_fd(" ", 2);
		i++;
	}
	ft_putstr_fd("\n", 2);
}

int is_debug_enabled(t_exec *exec)
/* Check if debug mode is enabled via environment variable */
{
	t_env_var	*debug;

	debug = get_env_var(exec, "MINISHELL_DEBUG");
	return (debug && debug->value
		&& (ft_strcmp(debug->value, "1") == 0
			|| ft_strcmp(debug->value, "true") == 0));
}
