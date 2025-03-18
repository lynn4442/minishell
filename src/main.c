/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:37:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/18 20:38:43 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//int main(void)
//{
//    t_exec *exec = init_exec();
//    if (!exec)
//    {
//        fprintf(stderr, "Failed to initialize exec structure\n");
//        return 1;
//    }

//    // Get the current user's home directory and username
//    char *home_dir = getenv("HOME");
//    char *username = getenv("USER");
//    if (!home_dir || !username) {
//        fprintf(stderr, "Environment variables not set\n");
//        free_exec(exec);
//        return 1;
//    }

//    // Add initial environment variables
//    add_env_var(exec, "HOME", home_dir);
//    add_env_var(exec, "PWD", home_dir);
//    add_env_var(exec, "OLDPWD", "/tmp"); // or any other default directory
//    add_env_var(exec, "USER", username);

//    // Test cd
//    printf("Testing cd:\n");
//    ft_cd(exec, "/tmp");  // Change to /tmp
//    ft_cd(exec, "~");     // Change to home directory
//    ft_cd(exec, "-");     // Change to OLDPWD

//    // Test echo
//    printf("\nTesting echo:\n");
//    char *echo_args[] = {"echo", "-n", "Hello, $USER!", NULL};
//    t_cmd_node *echo_cmd = create_cmd_node(echo_args);
//    ft_echo(echo_cmd, exec->env_list, exec);
//    free_cmd_node(echo_cmd);

//    // Test env
//    printf("\nTesting env:\n");
//    ft_env(exec);

//    // Test export
//    printf("\nTesting export:\n");
//    ft_export(exec->env_list);

//    // Test unset
//    printf("\nTesting unset:\n");
//    unset_env_var(exec, "OLDPWD");
//    ft_env(exec);  // Should not show OLDPWD

//    // Test pwd
//    printf("\nTesting pwd:\n");
//    ft_pwd(exec);

//    // Free everything
//    free_exec(exec);

//    return 0;
//}

//void parse_and_execute(t_exec *exec, char *input)
//{
//    // Here you would parse the input and create the command list
//    // For simplicity, let's assume the input is a simple command without pipes or redirections
//    char **args = ft_split(input, ' ');
//    t_cmd_node *cmd = create_cmd_node(args);
//    if (!cmd)
//    {
//        printf("Error: Failed to create command node\n");
//        return;
//    }

//    // Add the command to the execution structure
//    exec->cmd_list = cmd;

//    // Execute the command
//    if (ft_strcmp(cmd->arr[0], "echo") == 0)
//        ft_echo(cmd, exec->env_list, exec);
//    else if (ft_strcmp(cmd->arr[0], "cd") == 0)
//        ft_cd(exec, cmd->arr[1]);
//    else if (ft_strcmp(cmd->arr[0], "pwd") == 0)
//        ft_pwd(exec);
//    else if (ft_strcmp(cmd->arr[0], "env") == 0)
//        ft_env(exec);
//    else if (ft_strcmp(cmd->arr[0], "export") == 0)
//		ft_export(exec->env_list);
//    else if (ft_strcmp(cmd->arr[0], "unset") == 0)
//        unset_env_var(exec, cmd->arr[1]);
//    else if (ft_strcmp(cmd->arr[0], "exit") == 0)
//        ft_exit(cmd->arr, exec->exit_status);
//    else
//    {
//        printf("minishell: command not found: %s\n", cmd->arr[0]);
//        exec->exit_status = 127; // Command not found
//    }
//    // Free the command node after execution
//    free_cmd_node(cmd);
//    exec->cmd_list = NULL;
//}

//void parse_and_execute(t_exec *exec, char *input)
//{
//    // Here you would parse the input and create the command list
//    // For simplicity, let's assume the input is a simple command without pipes or redirections
//    char **args = ft_split(input, ' ');
//    t_cmd_node *cmd = create_cmd_node(args);
//    if (!cmd)
//    {
//        printf("Error: Failed to create command node\n");
//        return;
//    }

//    // Add the command to the execution structure
//    exec->cmd_list = cmd;

//    // Handling the export command
//    if (ft_strncmp(cmd->arr[0], "export", 6) == 0)
//    {
//        // If export is followed by a space or something else, handle normally
//        char *arg = cmd->arr[0] + 6;
//        while (*arg == ' ') // Skip any leading spaces after the command
//            arg++;

//        if (*arg == '\0') // If no argument, list environment variables
//        {
//            ft_export(exec->env_list);
//        }
//        else // If there is an argument, handle it as a variable assignment
//        {
//            handle_export(arg, &exec->env_list);
//        }
//    }
//    // Execute other commands
//    else if (ft_strcmp(cmd->arr[0], "echo") == 0)
//        ft_echo(cmd, exec->env_list, exec);
//    else if (ft_strcmp(cmd->arr[0], "cd") == 0)
//        ft_cd(exec, cmd->arr[1]);
//    else if (ft_strcmp(cmd->arr[0], "pwd") == 0)
//        ft_pwd(exec);
//    else if (ft_strcmp(cmd->arr[0], "env") == 0)
//        ft_env(exec);
//    else if (ft_strcmp(cmd->arr[0], "unset") == 0)
//        unset_env_var(exec, cmd->arr[1]);
//    else if (ft_strcmp(cmd->arr[0], "exit") == 0)
//        ft_exit(cmd->arr, exec->exit_status);
//    else
//    {
//        printf("minishell: command not found: %s\n", cmd->arr[0]);
//        exec->exit_status = 127; // Command not found
//    }

//    // Free the command node after execution
//    free_cmd_node(cmd);
//    exec->cmd_list = NULL;
//}

void parse_and_execute(t_exec *exec, char *input)
{
	if (ft_strncmp(input, "export", 6) == 0)
	{
		// If export is followed by a space or something else, handle normally
		char *arg = input + 6;
		while (*arg == ' ') // Skip any leading spaces after the command
			arg++;

		if (*arg == '\0') // If no argument, list environment variables
		{
			ft_export(exec->env_list); // This should print the environment only if `export` has no arguments.
		}
		else // If there is an argument, handle it as a variable assignment
		{
			handle_export(arg, &exec->env_list); // This should handle the variable assignment
		}
	}
	else
	{
		// Other commands...
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
			ft_env(exec);
		else if (ft_strcmp(cmd->arr[0], "export") == 0)
			ft_export(exec->env_list);
		else if (ft_strcmp(cmd->arr[0], "unset") == 0)
			unset_env_var(exec, cmd->arr[1]);
		else if (ft_strcmp(cmd->arr[0], "exit") == 0)
			ft_exit(cmd->arr, exec->exit_status);
		else
		{
			printf("%s: command not found\n", cmd->arr[0]);
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
		char *equal_sign = strchr(env_var, '=');
		if (equal_sign)
		{
			size_t key_len = equal_sign - env_var;
			char *key = ft_strndup(env_var, key_len);
			char *value = strdup(equal_sign + 1);
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
		if (strlen(input) > 0)
		{
			add_history(input);
			parse_and_execute(exec, input);
		}
		free(input);
	}

	free_exec(exec);
	return 0;
}
