/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:37:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/20 16:18:50 by lyoussef         ###   ########.fr       */
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
			handle_export(arg, &exec->env_list);
	}
	else
	{
		char **args = ft_split(input, ' ');
		t_cmd_node *cmd = create_cmd_node(args);
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
		free_cmd_node(cmd);
		exec->cmd_list = NULL;
	}
}

int main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	t_exec *exec = malloc(sizeof(t_exec));
	if (!exec)
	{
		perror("malloc");
		return 1;
	}
	exec->cmd_list = NULL;
	exec->env_list = NULL;

	int i = 0;
	while (envp[i])
	{
		char *env_var = envp[i];
		char *equal_sign = ft_strchr(env_var, '=');
		if (equal_sign)
		{
			size_t key_len = equal_sign - env_var;
			char *key = ft_strndup(env_var, key_len);
			char *value = ft_strdup(equal_sign + 1);
			if (key && value)
			{
				update_env_var(exec, key, value, true);
				free(key);
				free(value);
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
		free(input);
	}

	free_exec(exec);
	return 0;
}


//int main(int ac, char **av, char **envp)
//{
//	(void)ac;
//	(void)av;

//	// Initialize the exec structure
//	t_exec *exec = malloc(sizeof(t_exec));
//	if (!exec)
//	{
//		perror("malloc");
//		return 1;
//	}
//	exec->cmd_list = NULL;
//	exec->env_list = NULL;
//	exec->exit_status = 0;  // Initialize exit status to 0 (Success)

//	// Populate environment variables
//	int i = 0;
//	while (envp[i])
//	{
//		char *env_var = envp[i];
//		char *equal_sign = ft_strchr(env_var, '=');
//		if (equal_sign)
//		{
//			size_t key_len = equal_sign - env_var;
//			char *key = ft_strndup(env_var, key_len);
//			char *value = ft_strdup(equal_sign + 1);
//			if (key && value)
//			{
//				update_env_var(exec, key, value, true);
//				free(key);
//				free(value);
//			}
//		}
//		i++;
//	}

//	// Set up signal handlers
//	//setup_signals();

//	char *input;
//	while (1)
//	{
//		// Check if a signal was received
//		if (g_signal_received)
//		{
//			g_signal_received = 0;  // Reset the signal flag
//			exec->exit_status = 130; // Set exit status for Ctrl+C (SIGINT)
//			continue;  // Start the next iteration of the loop
//		}
//		input = readline("minishell> ");
//		if (!input)
//		{
//			// Handle EOF (Ctrl+D)
//			printf("exit\n");
//			exec->exit_status = 0; // Exit normally
//			break;
//		}
//		if (ft_strlen(input) > 0)
//		{
//			add_history(input);
//			parse_and_execute(exec, input);
//		}
//		free(input);
//	}
//	free_exec(exec);
//	return exec->exit_status;  // Return the exit status
//}
