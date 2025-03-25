/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:37:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 11:11:53 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**get_path_from_env(t_exec *exec)
{
	t_env_var	*current;

	current = exec->env_list;
	while (current)
	{
		if (ft_strcmp(current->key, "PATH") == 0)
			return (ft_split(current->value, ':', &exec->gc));
		current = current->next;
	}
	return (NULL);
}

//before dividing it
void	parse_and_execute(t_exec *exec, t_cmd_node *cmd, char **envp)
{
	char	**path_dirs;
	char	*full_path;
	int		found;
	int		i;

	i = 0;
	found = 0;
	if (!cmd || !cmd->arr)
	{
		printf("Error: Invalid command node\n");
		return ;
	}
	if (ft_strcmp(cmd->arr[0], "echo") == 0)
		ft_echo(cmd, exec->env_list, exec);
	else if (ft_strcmp(cmd->arr[0], "cd") == 0)
		ft_cd(exec, cmd->arr[1]);
	else if (ft_strcmp(cmd->arr[0], "pwd") == 0)
		ft_pwd(exec);
	else if (ft_strcmp(cmd->arr[0], "env") == 0)
		ft_env(exec, cmd->arr);
	else if (ft_strcmp(cmd->arr[0], "export") == 0)
	{
		if (cmd->arr[1] == NULL)
			ft_export(exec->env_list);
		else
		{
			int i = 1;
			while (cmd->arr[i])
			{
				if (ft_strchr(cmd->arr[i], '=') == NULL)
				{
					handle_export(&exec->gc, &exec->env_list, ft_strjoin(cmd->arr[i], "=", &exec->gc));
				}
				else
				{
					handle_export(&exec->gc, &exec->env_list, cmd->arr[i]);
				}
				i++;
			}
		}
	}
	else if (ft_strcmp(cmd->arr[0], "unset") == 0)
		unset_env_var(exec, cmd->arr[1]);
	else if (ft_strcmp(cmd->arr[0], "exit") == 0)
		ft_exit(cmd->arr, exec->exit_status);
	else
	{
		path_dirs = get_path_from_env(exec);
		if (!path_dirs)
		{
			printf("minihell: %s: command not found\n", cmd->arr[0]);
			exec->exit_status = 127;
			return ;
		}
		while (path_dirs[i])
		{
			full_path = ft_strjoin(path_dirs[i], "/", &exec->gc);
			if (!full_path)
			{
				printf("minihell: memory allocation failed\n");
				exec->exit_status = 1;
				return ;
			}
			if (access(full_path, X_OK) == 0)
			{
				execute_command(cmd, &exec->gc, envp);
				found = 1;
				break ;
			}
			i++;
		}
		if (!found)
		{
			printf("minihell: %s: command not found\n", cmd->arr[0]);
			exec->exit_status = 127;
		}
	}
}

int	main(int ac, char **av, char **envp)
{
	int			i;
	t_gc		hello;
	t_exec		*exec;
	char		*env_var;
	char		*equal_sign;
	char		*key;
	char		*value;
	char		*input;
	size_t		key_len;
	char		**args;
	t_cmd_node	*cmd;

	(void)ac;
	(void)av;
	i = 0;
	memset(&hello, 0, sizeof(t_gc));
	exec = ft_malloc(&hello, sizeof(t_exec));
	if (!exec)
	{
		perror("malloc");
		return (1);
	}
	init_exec(exec);
	while (envp[i])
	{
		env_var = envp[i];
		equal_sign = ft_strchr(env_var, '=');
		if (equal_sign)
		{
			key_len = equal_sign - env_var;
			key = ft_strndup(&exec->gc, env_var, key_len);
			value = ft_strdup(&exec->gc, equal_sign + 1);
			if (key && value)
				handle_export(&exec->gc, &exec->env_list, env_var);
		}
		i++;
	}
	while (1)
	{
		input = readline("minihell> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (ft_strlen(input) > 0)
		{
			add_history(input);
			args = ft_split(input, ' ', &exec->gc);
			if (args)
			{
				cmd = create_cmd_node(exec, args);
				parse_and_execute(exec, cmd, envp);
			}
		}
		if (ft_strcmp(input, "stop") == 0)
		{
			printf("hello");
			break ;
		}
	}
	ft_free_all(&exec->gc);
	return (0);
}
