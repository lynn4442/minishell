/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/04/08 09:45:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void execute_pipe(t_exec *exec, char ***commands, int cmd_count)
{
    int pipe_fds[cmd_count - 1][2];
    pid_t pids[cmd_count];
    t_cmd_node *cmd;
    char **env_array;

    // Convert environment list to array for execve
    env_array = convert_env_to_array(exec, &exec->gc);
    if (!env_array)
    {
        perror("minishell: environment conversion failed");
        return;
    }

    // Create pipes
    for (int i = 0; i < cmd_count - 1; i++)
    {
        if (pipe(pipe_fds[i]) == -1)
        {
            perror("pipe");
            return;
        }
    }

    // Execute commands
    for (int i = 0; i < cmd_count; i++)
    {
        cmd = create_cmd_node(exec, commands[i]);
        if (!cmd)
            continue;

        pids[i] = fork();
        if (pids[i] == 0)
        {
            // Child process
            if (i > 0)  // Not first command
            {
                close(pipe_fds[i-1][1]);
                dup2(pipe_fds[i-1][0], STDIN_FILENO);
                close(pipe_fds[i-1][0]);
            }
            if (i < cmd_count - 1)  // Not last command
            {
                close(pipe_fds[i][0]);
                dup2(pipe_fds[i][1], STDOUT_FILENO);
                close(pipe_fds[i][1]);
            }

            // Handle redirections
            if (cmd->in)
            {
                int fd = open(cmd->in, O_RDONLY);
                if (fd == -1)
                {
                    perror("minishell");
                    exit(1);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            if (cmd->out)
            {
                int flags = O_WRONLY | O_CREAT;
                flags |= cmd->append ? O_APPEND : O_TRUNC;
                int fd = open(cmd->out, flags, 0644);
                if (fd == -1)
                {
                    perror("minishell");
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            if (is_builtin_command(cmd->arr[0]))
            {
                handle_builtin_command(cmd->exec, cmd);
                exit(cmd->exec->exit_status);
            }
            else
            {
                char *cmd_path = find_command_path(cmd->exec, cmd->arr[0]);
                if (!cmd_path)
                {
                    ft_putstr_fd("minishell: ", 2);
                    ft_putstr_fd(cmd->arr[0], 2);
                    ft_putstr_fd(": command not found\n", 2);
                    exit(127);
                }
                execve(cmd_path, cmd->arr, env_array);
                perror("execve");
                exit(1);
            }
        }
        else if (pids[i] < 0)
        {
            perror("fork");
            return;
        }
    }

    // Parent process: close all pipe fds
    for (int i = 0; i < cmd_count - 1; i++)
    {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
    }

    // Wait for all processes
    int status;
    for (int i = 0; i < cmd_count; i++)
    {
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status))
            exec->exit_status = WEXITSTATUS(status);
    }
}

// Helper function to check if a command contains a pipe
int	has_pipe(t_cmd_node *cmd)
{
    return (cmd && cmd->type == PIPE);
}

// Main function to handle command execution with potential pipes
void	execute_with_pipes(t_exec *exec, t_cmd_node *cmd_list)
{
    t_cmd_node *current;
    int cmd_count;
    char ***commands;
    int i;

    // Count commands
    cmd_count = 0;
    current = cmd_list;
    while (current)
    {
        cmd_count++;
        current = current->next;
    }

    // Allocate array for commands
    commands = ft_malloc(&exec->gc, (cmd_count + 1) * sizeof(char **));
    if (!commands)
        return;

    // Fill array with commands
    i = 0;
    current = cmd_list;
    while (current)
    {
        commands[i] = current->arr;
        i++;
        current = current->next;
    }
    commands[i] = NULL;

    execute_pipe(exec, commands, cmd_count);
} 