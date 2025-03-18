/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:12:03 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/18 10:12:42 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env_var *create_env_var(char *name, char *value)
{
	t_env_var *env = malloc(sizeof(t_env_var));
	if (!env)
		return NULL;
	env->key = ft_strdup(name);
	env->value = ft_strdup(value);
	if (!env->key || !env->value)
	{
		free(env->key);
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
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

t_cmd_node *create_cmd_node(char **args)
{
	t_cmd_node *cmd = malloc(sizeof(t_cmd_node));
	int count;
	int i;

	i = 0;
	count = 0;
	if (!cmd)
		return NULL;
	while (args && args[count])
		count++;
	cmd->arr = malloc((count + 1) * sizeof(char *));
	if (!cmd->arr)
		return (free(cmd), NULL);
	while (i < count)
	{
		cmd->arr[i] = ft_strdup(args[i]);
		i++;
	}
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

void free_cmd_node(t_cmd_node *cmd)
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
	free_cmd_node(exec->cmd_list);
	free(exec);
}

void add_env_var(t_exec *exec, char *name, char *value)
{
	t_env_var *new_var;

	new_var = create_env_var(name, value);
	if (!new_var)
		return;
	if (!exec->env_list)
		exec->env_list = new_var;
	else
	{
		t_env_var *temp = exec->env_list;
		while (temp->next)
			temp = temp->next;
		temp->next = new_var;
		new_var->prev = temp;
	}
}
