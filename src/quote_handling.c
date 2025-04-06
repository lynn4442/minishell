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
        printf("%s", exit_code_str);
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
            printf("%s", value);
        *i = end;
    }
    else
    {
        printf("$");
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
        if (arg[i] == '\\' && !escaped)
        {
            escaped = 1;
            i++;
            continue;
        }
        if (escaped)
        {
            if (arg[i] == ' ')
            {
                printf(" ");
            }
            else
            {
                printf("%c", arg[i]);
            }
            escaped = 0;
            i++;
            continue;
        }
        if (is_quote(arg[i]) && quote_type == '\0')
        {
            quote_type = arg[i];
            i++;
            continue;
        }
        else if (arg[i] == quote_type)
        {
            quote_type = '\0';
            i++;
            continue;
        }
        if (arg[i] == '$' && (quote_type == '\0' || quote_type == '"') && arg[i + 1])
        {
            handle_env_variable(arg, &i, env, exec);
            continue;
        }
        if (arg[i] == ' ')
        {
            if (quote_type)
            {
                printf(" ");
            }
            else
            {
                if (space_start == -1)
                {
                    space_start = i;
                    printf(" ");
                }
            }
        }
        else
        {
            space_start = -1;
            printf("%c", arg[i]);
        }
        i++;
    }
}

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
    return (quote_type == '\0');
}
