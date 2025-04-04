/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/28 10:00:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int is_quote(char c)
{
    return (c == '\'' || c == '"');
}

static void handle_env_variable(const char *arg, int *i, t_env_var *env, t_exec *exec)
{
    if (arg[*i + 1] == '?')
    {
        char *exit_code_str = ft_itoa(exec->exit_status, &exec->gc);
        write(STDOUT_FILENO, exit_code_str, ft_strlen(exit_code_str));
        *i += 2;
        return;
    }

    int start = *i + 1;
    int end = start;
    while (arg[end] && (ft_isalnum(arg[end]) || arg[end] == '_'))
        end++;

    if (end > start)
    {
        char *var_name = ft_strndup(&exec->gc, arg + start, end - start);
        char *value = get_env_value(env, var_name);
        if (value)
            write(STDOUT_FILENO, value, ft_strlen(value));
        *i = end;
    }
    else
    {
        write(STDOUT_FILENO, "$", 1);
        (*i)++;
    }
}

void print_with_quote_handling(const char *arg, t_env_var *env, t_exec *exec)
{
    int i = 0;
    int len = ft_strlen(arg);
    char quote_type = '\0';
    int space_start = -1;
    int escaped = 0;

    while (i < len)
    {
        // Handle escape characters
        if (arg[i] == '\\' && !escaped)
        {
            escaped = 1;
            i++;
            continue;
        }
        
        // If we're in an escaped state, handle the escaped character
        if (escaped)
        {
            // For escaped spaces, just print a space
            if (arg[i] == ' ')
            {
                write(STDOUT_FILENO, " ", 1);
            }
            // For other escaped characters, print them as is
            else
            {
                write(STDOUT_FILENO, &arg[i], 1);
            }
            escaped = 0;
            i++;
            continue;
        }
        
        // Handle opening quotes
        if (is_quote(arg[i]) && quote_type == '\0')
        {
            quote_type = arg[i];
            i++;
            continue;
        }
        // Handle closing quotes
        else if (arg[i] == quote_type)
        {
            quote_type = '\0';
            i++;
            continue;
        }
        
        // Handle environment variables
        if (arg[i] == '$' && quote_type != '\'' && arg[i + 1])
        {
            handle_env_variable(arg, &i, env, exec);
            continue;
        }

        // Handle spaces differently based on whether we're in quotes or not
        if (arg[i] == ' ')
        {
            if (quote_type) // Inside quotes: preserve all spaces
            {
                write(STDOUT_FILENO, " ", 1);
            }
            else // Outside quotes: collapse multiple spaces into one
            {
                if (space_start == -1)
                {
                    space_start = i;
                    write(STDOUT_FILENO, " ", 1);
                }
            }
        }
        else
        {
            space_start = -1;
            write(STDOUT_FILENO, &arg[i], 1);
        }
        i++;
    }
}

// Add this function to check for unclosed quotes
int check_quotes(const char *input)
{
    int i = 0;
    char quote_type = '\0';
    int escaped = 0;

    while (input[i])
    {
        if (input[i] == '\\' && !escaped)
        {
            escaped = 1;
            i++;
            continue;
        }
        
        if (escaped)
        {
            escaped = 0;
            i++;
            continue;
        }
        
        if ((input[i] == '\'' || input[i] == '"') && quote_type == '\0')
            quote_type = input[i];
        else if (input[i] == quote_type)
            quote_type = '\0';
        i++;
    }

    // If quote_type is not '\0', we have an unclosed quote
    return (quote_type == '\0');
} 