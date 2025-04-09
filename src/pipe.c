/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/04/10 00:43:28 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char ***split_by_pipe(char *input, t_exec *exec)
{
    char **parts = ft_split(input, '|', &exec->gc);
    if (!parts)
        return NULL;

    // Count number of commands
    int cmd_count = 0;
    while (parts[cmd_count])
        cmd_count++;

    // Allocate array for commands
    char ***commands = ft_malloc(&exec->gc, (cmd_count + 1) * sizeof(char **));
    if (!commands)
        return NULL;

    // Process each command
    int i = 0;
    while (parts[i])
    {
        // Trim whitespace from the command
        char *trimmed = ft_strtrim(parts[i], " \t", &exec->gc);
        if (!trimmed)
            return NULL;
        
        // Check for empty commands
        if (ft_strlen(trimmed) == 0)
        {
            ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
            exec->exit_status = 2;
            return NULL;
        }
        
        // Use split_preserve_quotes to handle quoted arguments
        commands[i] = split_preserve_quotes(trimmed, &exec->gc);
        if (!commands[i])
            return NULL;

        // Create command node and set type
        t_cmd_node *cmd = create_cmd_node(exec, commands[i]);
        if (cmd)
        {
            cmd->type = PIPE;  // Set the command type to PIPE
            process_and_update_args(cmd, cmd->arr);
            parse_redirections(cmd, cmd->arr);
        }
        i++;
    }
    commands[i] = NULL;

    return commands;
}

char **split_preserve_quotes(const char *input, t_gc *gc)
{
    char **result;
    int i = 0;
    int start = 0;
    int count = 0;
    char quote = 0;
    int escaped = 0;
    int is_echo_command = 0;
    
    // First count the number of arguments
    while (input[i])
    {
        // Handle escape characters
        if (input[i] == '\\' && !escaped)
        {
            escaped = 1;
            i++;
            continue;
        }
        
        // If we're in an escaped state, just move to the next character
        if (escaped)
        {
            escaped = 0;
            i++;
            continue;
        }
        
        // Skip spaces if we're not in quotes
        while (input[i] && ft_isspace(input[i]) && !quote)
            i++;
        if (!input[i])
            break;
        
        // Mark start of an argument
        start = i;
        
        // Process until end of argument
        while (input[i])
        {
            // Handle escape characters
            if (input[i] == '\\' && !escaped)
            {
                escaped = 1;
                i++;
                continue;
            }
            
            // If we're in an escaped state, just move to the next character
            if (escaped)
            {
                escaped = 0;
                i++;
                continue;
            }
            
            if (input[i] == '\'' || input[i] == '"')
            {
                if (!quote)
                    quote = input[i];
                else if (quote == input[i])
                    quote = 0;
            }
            if (ft_isspace(input[i]) && !quote)
                break;
            i++;
        }
        if (start < i)
            count++;
        if (!input[i])
            break;
        i++;
    }

    // Allocate array
    result = ft_malloc(gc, sizeof(char *) * (count + 1));
    if (!result)
        return NULL;

    // Reset for second pass
    i = 0;
    count = 0;
    quote = 0;
    escaped = 0;

    // Second pass: copy arguments
    while (input[i])
    {
        // Handle escape characters
        if (input[i] == '\\' && !escaped)
        {
            escaped = 1;
            i++;
            continue;
        }
        
        // If we're in an escaped state, just move to the next character
        if (escaped)
        {
            escaped = 0;
            i++;
            continue;
        }
        
        while (input[i] && ft_isspace(input[i]) && !quote)
            i++;
        if (!input[i])
            break;

        start = i;
        while (input[i])
        {
            // Handle escape characters
            if (input[i] == '\\' && !escaped)
            {
                escaped = 1;
                i++;
                continue;
            }
            
            // If we're in an escaped state, just move to the next character
            if (escaped)
            {
                escaped = 0;
                i++;
                continue;
            }
            
            if (input[i] == '\'' || input[i] == '"')
            {
                if (!quote)
                    quote = input[i];
                else if (quote == input[i])
                    quote = 0;
            }
            if (ft_isspace(input[i]) && !quote)
                break;
            i++;
        }
        if (start < i)
        {
            char *arg = ft_strndup(gc, input + start, i - start);
            
            // Check if this is the echo command
            if (count == 0 && ft_strcmp(arg, "echo") == 0)
            {
                is_echo_command = 1;
                result[count] = arg;
            }
            // For echo command arguments, preserve quotes
            else if (is_echo_command)
            {
                result[count] = arg;
            }
            // For other commands, remove quotes if the argument is quoted
            else if (arg && (arg[0] == '\'' || arg[0] == '"') && 
                arg[ft_strlen(arg) - 1] == arg[0])
            {
                char *temp = ft_strndup(gc, arg + 1, ft_strlen(arg) - 2);
                result[count] = temp;
            }
            else
            {
                result[count] = arg;
            }
            count++;
        }
        if (!input[i])
            break;
        i++;
    }
    result[count] = NULL;
    return result;
} 