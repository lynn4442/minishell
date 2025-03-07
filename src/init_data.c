/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:12:03 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/07 20:42:40 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env_var *create_env_var(char *name, char *value)
{
	t_env_var *env = malloc(sizeof(t_env_var));
	if (!env)
		return NULL;
	env->name = ft_strdup(name);
	env->value = ft_strdup(value);
	if (!env->name || !env->value)
	{
		free(env->name);
		free(env->value);
		free(env);
		return NULL;
	}
	env->next = NULL;
	env->prev = NULL;
	return env;
}

void free_env_list(t_env_var *env)
{
	t_env_var *tmp;
	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
}

t_cmd_node *create_cmd_node(char **args)
{
	t_cmd_node *cmd = malloc(sizeof(t_cmd_node));
	int count;

	count = 0;
	if (!cmd)
		return NULL;
	while (args && args[count])
		count++;
	cmd->arr = malloc((count + 1) * sizeof(char *));
	if (!cmd->arr)
		return (free(cmd), NULL);
	for (int i = 0; i < count; i++)
		cmd->arr[i] = ft_strdup(args[i]);
	cmd->arr[count] = NULL;
	cmd->in = NULL;
	cmd->out = NULL;
	cmd->type = SMP_CMD;
	cmd->heredoc = NULL;
	cmd->append = 0;
	cmd->err = 0;
	cmd->ex_heredoc = 0;
	cmd->next = NULL;
	return cmd;
}

void free_cmd_list(t_cmd_node *cmd)
{
	t_cmd_node *tmp;
	while (cmd)
	{
		tmp = cmd;
		cmd = cmd->next;

		if (tmp->arr)
		{
			for (int i = 0; tmp->arr[i]; i++)
				free(tmp->arr[i]);
			free(tmp->arr);
		}
		free(tmp->in);
		free(tmp->out);
		free(tmp->heredoc);
		free(tmp);
	}
}

void free_elem_list(t_elem *elem)
{
	t_elem *tmp;
	while (elem)
	{
		tmp = elem;
		elem = elem->next;
		free(tmp->token);
		free(tmp);
	}
}

t_exec *init_exec()
{
	t_exec *exec = malloc(sizeof(t_exec));
	if (!exec)
		return NULL;
	exec->cmd_list = NULL;
	exec->env_list = NULL;
	exec->exit_status = 0;
	return exec;
}

void free_exec(t_exec *exec)
{
	if (!exec)
		return;
	free_env_list(exec->env_list);
	free_cmd_list(exec->cmd_list);
	free(exec);
}
