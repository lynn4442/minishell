/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/27 10:00:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int handle_output_redirection(t_cmd_node *cmd)
{
    int fd;

    if (!cmd->out)
        return STDOUT_FILENO;

    if (cmd->append)
        fd = open(cmd->out, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        fd = open(cmd->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1)
    {
        perror("minishell");
        return -1;
    }

    return fd;
}

void parse_redirections(t_cmd_node *cmd, char **args)
{
    int i = 0;
    int j;
    char **new_args;
    int arg_count = 0;

    // First pass: count valid arguments and handle redirections
    while (args[i])
    {
        if (ft_strcmp(args[i], ">") == 0 && args[i + 1])
        {
            // Create empty file even if it's not the last redirection
            int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd != -1)
                close(fd);
            
            // Only set as output if it's the last redirection
            cmd->out = args[i + 1];
            cmd->append = 0;
            i += 2;
            continue;
        }
        else if (ft_strcmp(args[i], ">>") == 0 && args[i + 1])
        {
            // Create file if it doesn't exist
            int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd != -1)
                close(fd);
            
            cmd->out = args[i + 1];
            cmd->append = 1;
            i += 2;
            continue;
        }
        else if (ft_strcmp(args[i], "<") == 0 && args[i + 1])
        {
            cmd->in = args[i + 1];
            i += 2;
            continue;
        }
        else if (ft_strcmp(args[i], ">") != 0 && 
                 ft_strcmp(args[i], ">>") != 0 &&
                 ft_strcmp(args[i], "<") != 0)
        {
            arg_count++;
        }
        i++;
    }

    // Allocate new args array
    new_args = ft_malloc(&cmd->exec->gc, sizeof(char *) * (arg_count + 1));
    if (!new_args)
        return;

    // Second pass: build new args array without redirections
    i = 0;
    j = 0;
    while (args[i])
    {
        if (ft_strcmp(args[i], ">") == 0 || 
            ft_strcmp(args[i], ">>") == 0 ||
            ft_strcmp(args[i], "<") == 0)
        {
            i += 2;
            continue;
        }
        
        // Handle quoted arguments
        if (args[i][0] == '"' || args[i][0] == '\'')
        {
            // Preserve the entire quoted string including spaces
            new_args[j++] = ft_strdup(&cmd->exec->gc, args[i]);
        }
        else
        {
            // For unquoted arguments, trim extra spaces
            char *trimmed = ft_strtrim(args[i], " \t");
            new_args[j++] = ft_strdup(&cmd->exec->gc, trimmed);
        }
        i++;
    }
    new_args[j] = NULL;
    cmd->arr = new_args;
}

// Returns -1 on error, fd on success
int setup_output_redirection(t_cmd_node *cmd, int *original_fd)
{
    int fd;

    if (!cmd->out)
        return 0;

    if (cmd->append)
        fd = open(cmd->out, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        fd = open(cmd->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        
    if (fd == -1)
    {
        perror("minishell");
        return -1;
    }

    // Save original stdout
    *original_fd = dup(STDOUT_FILENO);
    if (*original_fd == -1)
    {
        perror("minishell");
        close(fd);
        return -1;
    }

    // Redirect stdout to file
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("minishell");
        close(fd);
        close(*original_fd);
        return -1;
    }
    close(fd);
    return fd;
}

// Returns -1 on error, 0 on success
int restore_output_redirection(int original_fd)
{
    if (original_fd != -1)
    {
        if (dup2(original_fd, STDOUT_FILENO) == -1)
        {
            perror("minishell");
            close(original_fd);
            return -1;
        }
        close(original_fd);
    }
    return 0;
}

// Returns -1 on error, fd on success
int setup_input_redirection(t_cmd_node *cmd, int *original_fd)
{
    int fd;

    if (!cmd->in)
        return 0;

    fd = open(cmd->in, O_RDONLY);
    if (fd == -1)
    {
        perror("minishell");
        return -1;
    }

    // Save original stdin
    *original_fd = dup(STDIN_FILENO);
    if (*original_fd == -1)
    {
        perror("minishell");
        close(fd);
        return -1;
    }

    // Redirect stdin to file
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("minishell");
        close(fd);
        close(*original_fd);
        return -1;
    }
    close(fd);
    return fd;
}

// Returns -1 on error, 0 on success
int restore_input_redirection(int original_fd)
{
    if (original_fd != -1)
    {
        if (dup2(original_fd, STDIN_FILENO) == -1)
        {
            perror("minishell");
            close(original_fd);
            return -1;
        }
        close(original_fd);
    }
    return 0;
} 