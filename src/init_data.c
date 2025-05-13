/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:12:03 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 09:26:05 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env_var	*create_env_var(t_exec *exec, char *name, char *value)
{
	t_env_var	*env;

	env = ft_malloc(&exec->gc, sizeof(t_env_var));
	if (!env)
		return (NULL);
	env->key = ft_strdup(&exec->gc, name);
	env->value = ft_strdup(&exec->gc, value);
	if (!env->key || !env->value)
		return (NULL);
	env->next = NULL;
	env->prev = NULL;
	return (env);
}

/* Create a command node with memory allocation optimizations */
t_cmd_node	*create_cmd_node(t_exec *exec, char **args)
{
	t_cmd_node	*cmd;
	int			count;
	int			i;

	if (!exec || !args)
		return (NULL);

	// Count the number of arguments
	count = 0;
	while (args[count])
		count++;

	// Allocate command node
	cmd = ft_malloc(&exec->gc, sizeof(t_cmd_node));
	if (!cmd)
		return (NULL);

	// Initialize all fields to safe defaults
	cmd->arr = ft_malloc(&exec->gc, (count + 1) * sizeof(char *));
	cmd->in = NULL;
	cmd->out = NULL;
	cmd->type = SMP_CMD;
	cmd->heredoc = NULL;
	cmd->append = 0;
	cmd->err = 0;
	cmd->ex_heredoc = 0;
	cmd->exec = exec;
	cmd->next = NULL;

	// Copy arguments
	if (!cmd->arr)
		return (NULL);
	i = 0;
	while (i < count)
	{
		cmd->arr[i] = ft_strdup(&exec->gc, args[i]);
		if (!cmd->arr[i])
			return (NULL);
		i++;
	}
	cmd->arr[count] = NULL;

	return (cmd);
}

/* Initialize the exec structure */
void	init_exec(t_exec *exec)
{
	if (!exec)
		return;
	exec->cmd_list = NULL;
	exec->env_list = NULL;
	exec->exit_status = 0;
	exec->gc.head = NULL;
}

void	add_env_var(t_exec *exec, char *name, char *value)
{
	t_env_var	*new_var;
	t_env_var	*temp;

	new_var = create_env_var(exec, name, value);
	if (!new_var)
		return ;
	if (!exec->env_list)
		exec->env_list = new_var;
	else
	{
		temp = exec->env_list;
		while (temp->next)
			temp = temp->next;
		temp->next = new_var;
		new_var->prev = temp;
	}
}
