/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:37:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/17 09:49:39 by lyoussef         ###   ########.fr       */
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

void parse_and_execute(t_exec *exec, char *input)
{
    // Here you would parse the input and create the command list
    // For simplicity, let's assume the input is a simple command without pipes or redirections
    char **args = ft_split(input, ' ');
    t_cmd_node *cmd = create_cmd_node(args);
    if (!cmd)
    {
        printf("Error: Failed to create command node\n");
        return;
    }

    // Add the command to the execution structure
    exec->cmd_list = cmd;

    // Execute the command
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
        printf("minishell: command not found: %s\n", cmd->arr[0]);
        exec->exit_status = 127; // Command not found
    }
    // Free the command node after execution
    free_cmd_node(cmd);
    exec->cmd_list = NULL;
}

int main()
{
    t_exec *exec = init_exec();
    if (!exec)
    {
        printf("Error: Failed to initialize execution structure\n");
        return 1;
    }
    add_env_var(exec, "PATH", "/usr/local/bin:/usr/bin:/bin");
    add_env_var(exec, "HOME", "/home/user");
    add_env_var(exec, "USER", "user");
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
