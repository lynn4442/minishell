/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 19:55:04 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/22 13:47:20 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// heredoc on its own to take care of

# include "../minishell.h"

// <
void ft_input_red(t_cmd_node *cmd, t_gc *gc)
{
	if (cmd->in)
	{
		int fd = open(cmd->in, O_RDONLY);
		if (fd < 0)
		{
			perror("open (input)");
			ft_free_all(gc);
			exit(EXIT_FAILURE);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

// >>
void ft_output_append(t_cmd_node *cmd, t_gc *gc)
{
	if (cmd->out && cmd->append)
	{
		int fd = open(cmd->out, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			perror("open (append)");
			ft_free_all(gc);
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

// >
void ft_output_truncate(t_cmd_node *cmd, t_gc *gc)
{
	if (cmd->out && !cmd->append)
	{
		int fd = open(cmd->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror("open (truncate)");
			ft_free_all(gc);
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void handle_redirection(t_cmd_node *cmd, t_gc *gc)
{
	ft_input_redirection(cmd, gc);
	ft_output_append(cmd, gc);
	ft_output_truncate(cmd, gc);
}

void execute_command(t_cmd_node *cmd, t_gc *gc, char **envp)
{
	pid_t pid = fork();

	if (pid < 0)
	{
		perror("fork");
		ft_free_all(gc);
		exit(1);
	}
	else if (pid == 0)
	{
		handle_redirection(cmd, gc);
		execve(cmd->arr[0], cmd->arr, envp);
		perror("execve");
		ft_free_all(gc);
		exit(1);
	}
	else
		wait(NULL);
}
