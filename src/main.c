/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:37:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/07 19:53:54 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

int main()
{
    t_env_var *env_list = create_env_var("USER", "john");
    env_list->next = create_env_var("HOME", "/home/john");
    env_list->next->prev = env_list;
    env_list->next->next = create_env_var("SHELL", "/bin/bash");
    env_list->next->next->prev = env_list->next;

    char *test1[] = {"echo", "--n", NULL};
    char *test2[] = {"echo", "-n", NULL};
    char *test3[] = {"echo", "-----nnnn", "-n", NULL};

    printf("Test 1: ");
    ft_echo(create_cmd_node(test1), env_list, NULL);
    printf("\n");

    printf("Test 2: ");
    ft_echo(create_cmd_node(test2), env_list, NULL);
    printf("\n");

    printf("Test 3: ");
    ft_echo(create_cmd_node(test3), env_list, NULL);
    printf("\n");

	free_env_list(env_list);
    free_cmd_node(create_cmd_node(test1));
    free_cmd_node(create_cmd_node(test2));
    free_cmd_node(create_cmd_node(test3));
    return 0;
}
