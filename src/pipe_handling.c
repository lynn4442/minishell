/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/04/05 14:40:39 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void execute_pipe(t_exec *exec, char ***commands, int cmd_count, char **envp)
{
    if (cmd_count < 2)
        return;

    int **pipe_fds = ft_malloc(&exec->gc, (cmd_count - 1) * sizeof(int *));
    if (!pipe_fds)
        return;

    // Create pipes
    for (int i = 0; i < cmd_count - 1; i++)
    {
        pipe_fds[i] = ft_malloc(&exec->gc, 2 * sizeof(int));
        if (!pipe_fds[i] || pipe(pipe_fds[i]) == -1)
        {
            perror("pipe");
            return;
        }
    }

    // Create and execute processes
    pid_t *pids = ft_malloc(&exec->gc, cmd_count * sizeof(pid_t));
    if (!pids)
        return;

    for (int i = 0; i < cmd_count; i++)
    {
        t_cmd_node *cmd = create_cmd_node(exec, commands[i]);
        if (!cmd)
            continue;

        // Parse redirections
        parse_redirections(cmd, cmd->arr);
        process_and_update_args(cmd, cmd->arr);

        pids[i] = fork();
        if (pids[i] == 0)
        {
            // Set up signals for child process
            setup_child_signals();

            // Handle redirections first
            handle_redirection(cmd, &exec->gc);

            // Close all unused pipe ends first
            for (int j = 0; j < cmd_count - 1; j++)
            {
                if (j != i - 1 && j != i)
                {
                    close(pipe_fds[j][0]);
                    close(pipe_fds[j][1]);
                }
            }
            
            // First command
            if (i == 0)
            {
                if (!cmd->out)  // Only set up pipe if no output redirection
                {
                    dup2(pipe_fds[0][1], STDOUT_FILENO);
                    close(pipe_fds[0][0]);
                    close(pipe_fds[0][1]);
                }
            }
            // Last command
            else if (i == cmd_count - 1)
            {
                if (!cmd->in)  // Only set up pipe if no input redirection
                {
                    dup2(pipe_fds[i-1][0], STDIN_FILENO);
                    close(pipe_fds[i-1][0]);
                    close(pipe_fds[i-1][1]);
                }
            }
            // Middle commands
            else
            {
                if (!cmd->in)  // Only set up input pipe if no input redirection
                {
                    dup2(pipe_fds[i-1][0], STDIN_FILENO);
                    close(pipe_fds[i-1][0]);
                    close(pipe_fds[i-1][1]);
                }
                if (!cmd->out)  // Only set up output pipe if no output redirection
                {
                    dup2(pipe_fds[i][1], STDOUT_FILENO);
                    close(pipe_fds[i][0]);
                    close(pipe_fds[i][1]);
                }
            }

            // Execute command
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
                execve(cmd_path, cmd->arr, envp);
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
void	execute_with_pipes(t_exec *exec, t_cmd_node *cmd_list, char **envp)
{
    t_cmd_node *current;
    char ***commands;
    int cmd_count;

    // Count commands and collect them
    cmd_count = 0;
    current = cmd_list;
    while (current)
    {
        cmd_count++;
        current = current->next;
    }

    if (cmd_count < 2)
    {
        parse_and_execute(exec, cmd_list, envp);
        return;
    }

    // Allocate and fill commands array
    commands = ft_malloc(&exec->gc, (cmd_count + 1) * sizeof(char **));
    if (!commands)
        return;

    current = cmd_list;
    int i = 0;
    while (current)
    {
        commands[i++] = current->arr;
        current = current->next;
    }
    commands[i] = NULL;

    execute_pipe(exec, commands, cmd_count, envp);
} 