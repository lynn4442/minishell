/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/28 10:00:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char ***split_by_pipe(char *input, t_exec *exec)
{
    char ***commands = ft_malloc(&exec->gc, 3 * sizeof(char **));
    if (!commands)
        return NULL;

    char **parts = ft_split(input, '|', &exec->gc);
    if (!parts)
        return NULL;

    // Trim whitespace and split each part
    int i = 0;
    while (parts[i] && i < 2)
    {
        // Trim whitespace from the command
        char *trimmed = ft_strtrim(parts[i], " \t");
        if (!trimmed)
            return NULL;
        
        // Use split_preserve_quotes instead of ft_split to handle quoted arguments
        commands[i] = split_preserve_quotes(trimmed, &exec->gc);
        if (!commands[i])
            return NULL;
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
            
            // Remove quotes if the argument is quoted
            if (arg && (arg[0] == '\'' || arg[0] == '"') && 
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