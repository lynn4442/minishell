/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/27 10:00:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void child_process(t_cmd_node *cmd, int *pipe_fd, int is_input, char **envp)
{
    if (is_input)
    {
        // First command writes to pipe
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
    }
    else
    {
        // Second command reads from pipe
        close(pipe_fd[1]);
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);
    }

    // Handle any redirections in the command
    handle_redirection(cmd, &cmd->exec->gc);

    if (handle_builtin_command(cmd->exec, cmd))
        exit(cmd->exec->exit_status);

    char *cmd_path = find_command_path(cmd->exec, cmd->arr[0]);
    if (!cmd_path)
    {
        ft_putstr_fd("minishell: command not found: ", 2);
        ft_putstr_fd(cmd->arr[0], 2);
        ft_putstr_fd("\n", 2);
        exit(127);
    }

    execve(cmd_path, cmd->arr, envp);
    perror("execve");
    exit(126);
}

void execute_pipe(t_exec *exec, char **cmd1_args, char **cmd2_args, char **envp)
{
    int pipe_fd[2];
    pid_t pid1, pid2;

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        return;
    }

    // Create first command node
    t_cmd_node *cmd1 = create_cmd_node(exec, cmd1_args);
    if (!cmd1)
    {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return;
    }

    // Create second command node
    t_cmd_node *cmd2 = create_cmd_node(exec, cmd2_args);
    if (!cmd2)
    {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return;
    }

    // First child process
    pid1 = fork();
    if (pid1 == 0)
        child_process(cmd1, pipe_fd, 1, envp);
    else if (pid1 < 0)
    {
        perror("fork");
        return;
    }

    // Second child process
    pid2 = fork();
    if (pid2 == 0)
        child_process(cmd2, pipe_fd, 0, envp);
    else if (pid2 < 0)
    {
        perror("fork");
        return;
    }

    // Parent process
    close(pipe_fd[0]);
    close(pipe_fd[1]);

    // Wait for both processes
    int status;
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    if (WIFEXITED(status))
        exec->exit_status = WEXITSTATUS(status);
}

// Helper function to split command by pipe
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
        
        commands[i] = ft_split(trimmed, ' ', &exec->gc);
        if (!commands[i])
            return NULL;
        i++;
    }
    commands[i] = NULL;

    return commands;
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
    t_cmd_node *next;

    current = cmd_list;
    while (current)
    {
        if (has_pipe(current))
        {
            next = current->next;
            if (!next)
            {
                ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
                exec->exit_status = 2;
                return;
            }
            execute_pipe(exec, current->arr, next->arr, envp);
            current = next->next;  // Skip the next command as it's already executed
        }
        else
        {
            parse_and_execute(exec, current, envp);
            current = current->next;
        }
    }
} 