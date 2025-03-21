/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:37:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/21 05:50:25 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void parse_and_execute(t_exec *exec, char *input)
{
	if (ft_strncmp(input, "export", 6) == 0)
	{
		char *arg = input + 6;
		while (*arg == ' ')
			arg++;
		if (*arg == '\0')
			ft_export(exec->env_list);
		else
			handle_export(arg, &exec->env_list, &exec->gc);
	}
	else
	{
		char **args = ft_split(input, ' ',&exec->gc);
		t_cmd_node *cmd = create_cmd_node(exec, args);
		if (!cmd)
		{
			printf("Error: Failed to create command node\n");
			return;
		}
		exec->cmd_list = cmd;
		if (ft_strcmp(cmd->arr[0], "echo") == 0)
			ft_echo(cmd, exec->env_list, exec);
		else if (ft_strcmp(cmd->arr[0], "cd") == 0)
			ft_cd(exec, cmd->arr[1]);
		else if (ft_strcmp(cmd->arr[0], "pwd") == 0)
			ft_pwd(exec);
		else if (ft_strcmp(cmd->arr[0], "env") == 0)
			ft_env(exec, cmd->arr);
		else if (ft_strcmp(cmd->arr[0], "export") == 0)
			ft_export(exec->env_list);
		else if (ft_strcmp(cmd->arr[0], "unset") == 0)
			unset_env_var(exec, cmd->arr[1]);
		else if (ft_strcmp(cmd->arr[0], "exit") == 0)
			ft_exit(cmd->arr, exec->exit_status);
		else
		{
			printf("minihell: %s: command not found\n", cmd->arr[0]);
			exec->exit_status = 127;
		}
		exec->cmd_list = NULL;
	}
}

int main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	t_gc hello = {0};
	t_exec *exec = ft_malloc(&hello,sizeof(t_exec));
	if (!exec)
	{
		perror("malloc");
		return 1;
	}
	init_exec(&exec);
	int i = 0;
	while (envp[i])
	{
		char *env_var = envp[i];
		char *equal_sign = ft_strchr(env_var, '=');
		if (equal_sign)
		{
			size_t key_len = equal_sign - env_var;
			char *key = ft_strndup(&exec->gc,env_var, key_len);
			char *value = ft_strdup(&exec->gc, equal_sign + 1);
			if (key && value)
			{
				update_env_var(exec, key, value, true);
			}
		}
		i++;
	}
	char *input;
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break;
		}
		if (ft_strlen(input) > 0)
		{
			add_history(input);
			parse_and_execute(exec, input);
		}
		if (ft_strcmp(input, "stop") == 0)
		{
			printf("hello");
			break;
		}
	}
	ft_free_all(&exec->gc);
	return 0;
}
