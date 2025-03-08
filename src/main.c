/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:37:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/08 19:35:23 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

#include <stdlib.h> // for getenv

int main(void)
{
    t_exec *exec = init_exec();
    if (!exec)
    {
        fprintf(stderr, "Failed to initialize exec structure\n");
        return 1;
    }

    // Get the current user's home directory and username
    char *home_dir = getenv("HOME");
    char *username = getenv("USER");
    if (!home_dir || !username) {
        fprintf(stderr, "Environment variables not set\n");
        free_exec(exec);
        return 1;
    }

    // Add initial environment variables
    add_env_var(exec, "HOME", home_dir);
    add_env_var(exec, "PWD", home_dir);
    add_env_var(exec, "OLDPWD", "/tmp"); // or any other default directory
    add_env_var(exec, "USER", username);

    // Test cd
    printf("Testing cd:\n");
    ft_cd(exec, "/tmp");  // Change to /tmp
    ft_cd(exec, "~");     // Change to home directory
    ft_cd(exec, "-");     // Change to OLDPWD

    // Test echo
    printf("\nTesting echo:\n");
    char *echo_args[] = {"echo", "-n", "Hello, $USER!", NULL};
    t_cmd_node *echo_cmd = create_cmd_node(echo_args);
    ft_echo(echo_cmd, exec->env_list, exec);
    free_cmd_node(echo_cmd);

    // Test env
    printf("\nTesting env:\n");
    ft_env(exec);

    // Test export
    printf("\nTesting export:\n");
    export_no_options(exec->env_list);

    // Test unset
    printf("\nTesting unset:\n");
    unset_env_var(exec, "OLDPWD");
    ft_env(exec);  // Should not show OLDPWD

    // Test pwd
    printf("\nTesting pwd:\n");
    execute_pwd(exec);

    // Free everything
    free_exec(exec);

    return 0;
}
