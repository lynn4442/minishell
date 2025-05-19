/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/05/18 19:40:12 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Check if debug mode is enabled via environment variable */
static int is_debug_enabled(t_exec *exec)
{
    t_env_var *debug_var = get_env_var(exec, "MINISHELL_DEBUG");
    return (debug_var && debug_var->value && 
            (ft_strcmp(debug_var->value, "1") == 0 || 
             ft_strcmp(debug_var->value, "true") == 0));
}

/* Setup a single command's input from a pipe or file */
static void setup_pipe_input(t_cmd_node *cmd, int prev_pipe_fd)
{
    // Handle input redirection first
    if (cmd->in)
    {
        // Input redirection
        int fd = open(cmd->in, O_RDONLY);
        if (fd == -1)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd->in, 2);
            ft_putstr_fd(": No such file or directory\n", 2);
            // Don't exit, just set error status and continue
            cmd->exec->exit_status = 1;
            // Close pipe read end if it exists
            if (prev_pipe_fd != -1)
                close(prev_pipe_fd);
            return;
        }
        
        // Redirect stdin from the file
        if (dup2(fd, STDIN_FILENO) == -1)
            perror("dup2 input file");
        close(fd);
        
        // We're done with input - close pipe read end if it exists
        if (prev_pipe_fd != -1)
            close(prev_pipe_fd);
            
        return;
    }
    
    // If no input redirection, use pipe if available
    if (prev_pipe_fd != -1)
    {
        if (dup2(prev_pipe_fd, STDIN_FILENO) == -1)
            perror("dup2 pipe input");
        close(prev_pipe_fd);
    }
}

/* Setup a single command's output to a pipe or file */
static void setup_pipe_output(t_cmd_node *cmd, int next_pipe_fd)
{
    // Handle output redirection first
    if (cmd->out)
    {
        int flags = O_WRONLY | O_CREAT;
        flags |= cmd->append ? O_APPEND : O_TRUNC;
        
        // Open the output file
        int file_fd = open(cmd->out, flags, 0644);
        if (file_fd == -1)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd->out, 2);
            ft_putstr_fd(": Error opening output file\n", 2);
            exit(1);
        }

        // If we also have a pipe, we need to duplicate output to both
        if (next_pipe_fd != -1)
        {
            // Create a temporary pipe for tee-like functionality
            int tee_pipe[2];
            if (pipe(tee_pipe) == -1)
            {
                perror("pipe");
                close(file_fd);
                exit(1);
            }

            // Fork a child to handle the tee operation
            pid_t tee_pid = fork();
            if (tee_pid == -1)
            {
                perror("fork");
                close(file_fd);
                close(tee_pipe[0]);
                close(tee_pipe[1]);
                exit(1);
            }

            if (tee_pid == 0)
            {
                // Child process: read from pipe and write to both outputs
                char buffer[4096];
                ssize_t n;

                close(tee_pipe[1]);  // Close write end

                while ((n = read(tee_pipe[0], buffer, sizeof(buffer))) > 0)
                {
                    // Write to file
                    if (write(file_fd, buffer, n) != n)
                    {
                        perror("write to file");
                        exit(1);
                    }
                    // Write to next pipe
                    if (write(next_pipe_fd, buffer, n) != n)
                    {
                        perror("write to pipe");
                        exit(1);
                    }
                }

                close(tee_pipe[0]);
                close(file_fd);
                close(next_pipe_fd);
                exit(0);
            }
            else
            {
                // Parent process: redirect stdout to the tee pipe
                close(tee_pipe[0]);  // Close read end
                if (dup2(tee_pipe[1], STDOUT_FILENO) == -1)
                {
                    perror("dup2");
                    exit(1);
                }
                close(tee_pipe[1]);
                close(file_fd);
                // Don't close next_pipe_fd as the tee process needs it
            }
        }
        else
        {
            // No pipe, just redirect to file
            if (dup2(file_fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                exit(1);
            }
            close(file_fd);
        }
    }
    else if (next_pipe_fd != -1)
    {
        // No file redirection, just pipe
        if (dup2(next_pipe_fd, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(1);
        }
        close(next_pipe_fd);
    }
}

/* Execute a command in a pipeline (child process code) */
static void execute_pipeline_command(t_cmd_node *cmd, char **env_array)
{
    int debug = is_debug_enabled(cmd->exec);
    
    // Debug info to stderr (won't interfere with redirections)
    if (debug)
    {
        ft_putstr_fd("minishell: executing command: ", 2);
        for (int i = 0; cmd->arr && cmd->arr[i]; i++)
        {
            ft_putstr_fd(cmd->arr[i], 2);
            ft_putstr_fd(" ", 2);
        }
        ft_putstr_fd("\n", 2);
        
        if (cmd->in)
        {
            ft_putstr_fd("minishell: with input redirection from: ", 2);
            ft_putstr_fd(cmd->in, 2);
            ft_putstr_fd("\n", 2);
        }
        
        if (cmd->out)
        {
            ft_putstr_fd("minishell: with output redirection to: ", 2);
            ft_putstr_fd(cmd->out, 2);
            ft_putstr_fd(cmd->append ? " (append)" : " (truncate)", 2);
            ft_putstr_fd("\n", 2);
        }
    }
    
    // Verify command exists
    if (!cmd->arr || !cmd->arr[0])
    {
        ft_putstr_fd("minishell: empty command\n", 2);
        exit(1);
    }
    
    // Handle builtin commands
    if (is_builtin_command(cmd->arr[0]))
    {
        handle_builtin_command(cmd->exec, cmd);
        exit(cmd->exec->exit_status);
    }
    
    // Handle external commands
    char *cmd_path = find_command_path(cmd->exec, cmd->arr[0]);
    if (!cmd_path)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->arr[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        exit(127);
    }
    
    // Execute the command
    execve(cmd_path, cmd->arr, env_array);
    
    // If execve fails
    ft_putstr_fd("minishell: ", 2);
    perror(cmd->arr[0]);
    exit(126);
}

/* Close all unused pipe file descriptors */
static void close_all_pipes(int pipe_count, int pipes[][2])
{
    for (int i = 0; i < pipe_count; i++)
    {
        if (pipes[i][0] >= 0)  // Only close valid descriptors
            close(pipes[i][0]);
        if (pipes[i][1] >= 0)  // Only close valid descriptors
            close(pipes[i][1]);
    }
}

/* Main function to execute a pipeline of commands */
void execute_with_pipes(t_exec *exec, t_cmd_node *cmd_list)
{
    t_cmd_node *current = cmd_list;
    int cmd_count = 0;
    int pipe_count = 0;
    int debug = is_debug_enabled(exec);
    
    // Count commands in pipeline
    while (current)
    {
        cmd_count++;
        current = current->next;
    }
    
    if (cmd_count <= 0)
        return;
    
    // Debug output - show the command pipeline
    if (debug)
    {
        ft_putstr_fd("minishell: setting up pipeline with ", 2);
        ft_putstr_fd(ft_itoa(cmd_count, &exec->gc), 2);
        ft_putstr_fd(" commands\n", 2);
        
        // Print command details
        current = cmd_list;
        int i = 0;
        while (current)
        {
            ft_putstr_fd("minishell: cmd", 2);
            ft_putstr_fd(ft_itoa(i++, &exec->gc), 2);
            ft_putstr_fd(": ", 2);
            
            if (current->arr && current->arr[0])
            {
                for (int j = 0; current->arr[j]; j++)
                {
                    ft_putstr_fd(current->arr[j], 2);
                    ft_putstr_fd(" ", 2);
                }
            }
            
            if (current->out)
            {
                if (current->append)
                    ft_putstr_fd(">> ", 2);
                else
                    ft_putstr_fd("> ", 2);
                ft_putstr_fd(current->out, 2);
                ft_putstr_fd(" ", 2);
            }
            
            if (current->in)
            {
                ft_putstr_fd("< ", 2);
                ft_putstr_fd(current->in, 2);
                ft_putstr_fd(" ", 2);
            }
            
            ft_putstr_fd("\n", 2);
            current = current->next;
        }
    }
    
    // If only one command, use the standard execution
    if (cmd_count == 1)
    {
        execute_command_supreme(exec, cmd_list);
        return;
    }
    
    // For multiple commands, we need pipes
    pipe_count = cmd_count - 1;
    int pipes[pipe_count][2];
    pid_t pids[cmd_count];
    
    // Initialize pipe descriptors to -1
    for (int i = 0; i < pipe_count; i++)
    {
        pipes[i][0] = -1;
        pipes[i][1] = -1;
    }
    
    // Create all pipes
    for (int i = 0; i < pipe_count; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe");
            close_all_pipes(i, pipes); // Close any pipes created so far
            return;
        }
    }
    
    // Convert environment to array for child processes
    char **env_array = convert_env_to_array(exec, &exec->gc);
    if (!env_array)
    {
        ft_putstr_fd("minishell: environment conversion failed\n", 2);
        close_all_pipes(pipe_count, pipes);
        return;
    }
    
    // Fork for each command
    current = cmd_list;
    for (int i = 0; i < cmd_count; i++)
    {
        // Debug output - Command being executed
        if (debug)
        {
            ft_putstr_fd("minishell: setting up command ", 2);
            ft_putstr_fd(ft_itoa(i+1, &exec->gc), 2);
            ft_putstr_fd("/", 2);
            ft_putstr_fd(ft_itoa(cmd_count, &exec->gc), 2);
            ft_putstr_fd("\n", 2);
        }
        
        pids[i] = fork();
        
        if (pids[i] == -1)
        {
            perror("fork");
            close_all_pipes(pipe_count, pipes);
            return;
        }
        
        if (pids[i] == 0)
        {
            // Child process
            setup_child_signals();
            
            // Debug - show command details
            if (debug)
            {
                ft_putstr_fd("minishell: child process for command: ", 2);
                if (current->arr && current->arr[0])
                    ft_putstr_fd(current->arr[0], 2);
                ft_putstr_fd("\n", 2);
            }
            
            // Setup input (from previous pipe or file)
            int prev_pipe_fd = (i > 0) ? pipes[i - 1][0] : -1;
            setup_pipe_input(current, prev_pipe_fd);
            
            // Setup output (to next pipe or file)
            int next_pipe_fd = (i < pipe_count) ? pipes[i][1] : -1;
            setup_pipe_output(current, next_pipe_fd);
            
            // Close all other pipe file descriptors
            close_all_pipes(pipe_count, pipes);
            
            // Execute the command
            execute_pipeline_command(current, env_array);
            
            // Should never reach here
            exit(1);
        }
        
        // Parent process continues
        current = current->next;
    }
    
    // Parent: close all pipe file descriptors
    close_all_pipes(pipe_count, pipes);
    
    // Wait for all children to finish
    int status;
    int last_status = 0;
    
    for (int i = 0; i < cmd_count; i++)
    {
        waitpid(pids[i], &status, 0);
        
        // Debug - show exit status
        if (debug)
        {
            ft_putstr_fd("minishell: command ", 2);
            ft_putstr_fd(ft_itoa(i+1, &exec->gc), 2);
            ft_putstr_fd(" finished with status ", 2);
            ft_putstr_fd(ft_itoa(WEXITSTATUS(status), &exec->gc), 2);
            ft_putstr_fd("\n", 2);
        }
        
        // Save status of the last command in the pipeline
        if (i == cmd_count - 1)
        {
            if (WIFEXITED(status))
                last_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                last_status = 128 + WTERMSIG(status);
        }
    }
    
    // Set the exit status from the last command in the pipeline
    exec->exit_status = last_status;
    
    // Debug - pipeline execution complete
    if (debug)
    {
        ft_putstr_fd("minishell: pipeline execution complete\n", 2);
    }
}

/* Check if command includes a pipe */
int has_pipe(t_cmd_node *cmd)
{
    if (!cmd)
        return 0;
    
    return (cmd->type == PIPE);
}

/* Execute a pipe from string commands (old interface maintained for compatibility) */
void execute_pipe(t_exec *exec, char ***commands, int cmd_count)
{
    // Use the new centralized parser for pipe commands
    // This function is kept for backward compatibility
    char *input = NULL;
    t_cmd_node *pipe_cmds = NULL;
    
    // Combine the command parts into a single input string
    for (int i = 0; i < cmd_count; i++)
    {
        // Combine the command parts
        char *cmd_str = NULL;
        for (int j = 0; commands[i][j]; j++)
        {
            if (!cmd_str)
                cmd_str = ft_strdup(&exec->gc, commands[i][j]);
            else
            {
                char *tmp = ft_strjoin(cmd_str, " ", &exec->gc);
                cmd_str = ft_strjoin(tmp, commands[i][j], &exec->gc);
            }
        }
        
        // Add to the input string with pipe separators
        if (!input)
            input = cmd_str;
        else
        {
            char *tmp = ft_strjoin(input, " | ", &exec->gc);
            input = ft_strjoin(tmp, cmd_str, &exec->gc);
        }
    }
    
    // Parse the reconstructed input with our parser
    if (input)
        pipe_cmds = parse_piped_commands(input, exec);
    
    // Execute the pipe commands
    if (pipe_cmds)
        execute_with_pipes(exec, pipe_cmds);
} 