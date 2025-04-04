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

void child_process(t_cmd_node *cmd, int *pipe_fd, int is_first, char **envp)
{
    int original_fd;

    // Set up pipe
    if (is_first)
    {
        close(pipe_fd[0]);  // Close read end
        if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(1);
        }
        close(pipe_fd[1]);
    }
    else
    {
        close(pipe_fd[1]);  // Close write end
        if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit(1);
        }
        close(pipe_fd[0]);
    }

    // Handle redirections
    if (cmd->in || cmd->out)
    {
        if (cmd->in)
        {
            if (!setup_input_redirection(cmd, &original_fd))
                exit(1);
        }
        if (cmd->out)
        {
            if (!setup_output_redirection(cmd, &original_fd))
                exit(1);
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

    // Parse redirections for both commands
    parse_redirections(cmd1, cmd1->arr);
    parse_redirections(cmd2, cmd2->arr);

    // Process and update arguments after parsing redirections
    process_and_update_args(cmd1, cmd1->arr);
    process_and_update_args(cmd2, cmd2->arr);

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