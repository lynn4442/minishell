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

void process_and_update_args(t_cmd_node *cmd, char **args)
{
    int i = 0;
    int j = 0;
    char **new_args;
    int arg_count = 0;
    char *last_out = NULL;
    int last_append = 0;

    // First pass: count arguments and create all output files
    while (args[i])
    {
        if ((ft_strcmp(args[i], ">") == 0 || 
             ft_strcmp(args[i], ">>") == 0 ||
             ft_strcmp(args[i], "<") == 0) && args[i + 1])
        {
            if (ft_strcmp(args[i], ">") == 0)
            {
                // Create the file
                int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd != -1)
                    close(fd);
                
                // Remember the last output file
                last_out = args[i + 1];
                last_append = 0;
            }
            else if (ft_strcmp(args[i], ">>") == 0)
            {
                // Create the file
                int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fd != -1)
                    close(fd);
                
                // Remember the last output file
                last_out = args[i + 1];
                last_append = 1;
            }
            else if (ft_strcmp(args[i], "<") == 0)
                cmd->in = args[i + 1];
            i += 2;
        }
        else
        {
            arg_count++;
            i++;
        }
    }
    
    // Set the last output file
    cmd->out = last_out;
    cmd->append = last_append;
    
    // Second pass: create new args array without redirections
    new_args = ft_malloc(&cmd->exec->gc, sizeof(char *) * (arg_count + 1));
    if (!new_args)
        return;
    i = 0;
    j = 0;
    while (args[i])
    {
        if ((ft_strcmp(args[i], ">") == 0 || 
             ft_strcmp(args[i], ">>") == 0 ||
             ft_strcmp(args[i], "<") == 0) && args[i + 1])
        {
            i += 2;
            continue;
        }
        new_args[j++] = ft_strdup(&cmd->exec->gc, args[i++]);
    }
    new_args[j] = NULL;
    cmd->arr = new_args;
}

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
    *original_fd = dup(STDOUT_FILENO);
    if (*original_fd == -1)
    {
        perror("minishell");
        close(fd);
        return -1;
    }
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
    *original_fd = dup(STDIN_FILENO);
    if (*original_fd == -1)
    {
        perror("minishell");
        close(fd);
        return -1;
    }
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