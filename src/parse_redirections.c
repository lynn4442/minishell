/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/27 10:00:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void parse_redirections(t_cmd_node *cmd, char **args)
{
    int i;
    int last_out_index;

    i = 0;
    last_out_index = -1;

    // First pass: find the last output redirection
    while (args[i])
    {
        if ((ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0) && args[i + 1])
        {
            last_out_index = i;
        }
        i++;
    }

    i = 0;
    while (args[i])
    {
        if (ft_strcmp(args[i], "<") == 0 && args[i + 1])
        {
            cmd->in = args[i + 1];
            // Remove redirection from args
            while (args[i])
            {
                args[i] = args[i + 2];
                i++;
            }
            i = 0;
            continue;
        }
        else if ((ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0) && args[i + 1])
        {
            // Only set output if this is the last output redirection
            if (i == last_out_index)
            {
                cmd->out = args[i + 1];
                cmd->append = (ft_strcmp(args[i], ">>") == 0);
            }
            // Remove redirection from args
            while (args[i])
            {
                args[i] = args[i + 2];
                i++;
            }
            i = 0;
            continue;
        }
        i++;
    }
} 