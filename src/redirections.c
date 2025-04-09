/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 19:55:04 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/22 16:03:17 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// heredoc on its own to take care of

# include "../minishell.h"

// <
void ft_input_redirection(t_cmd_node *cmd, t_gc *gc)
{
	if (cmd->in)
	{
		int fd = open(cmd->in, O_RDONLY);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->in, 2);
			ft_putstr_fd(": 4No such file or directory\n", 2);
			ft_free_all(gc);
			exit(1);
		}
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("minishell");
			close(fd);
			ft_free_all(gc);
			exit(1);
		}
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
			ft_putstr_fd("minishell: ", 2);
			perror(cmd->out);
			ft_free_all(gc);
			exit(1);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("minishell");
			close(fd);
			ft_free_all(gc);
			exit(1);
		}
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
			ft_putstr_fd("minishell: ", 2);
			perror(cmd->out);
			ft_free_all(gc);
			exit(1);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("minishell");
			close(fd);
			ft_free_all(gc);
			exit(1);
		}
		close(fd);
	}
}

void handle_redirection(t_cmd_node *cmd, t_gc *gc)
{
	// Handle input redirection first
	if (cmd->in)
	{
		int fd = open(cmd->in, O_RDONLY);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->in, 2);
			ft_putstr_fd(": 5No such file or directory\n", 2);
			ft_free_all(gc);
			exit(1);
		}
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("minishell");
			close(fd);
			ft_free_all(gc);
			exit(1);
		}
		close(fd);
	}

	// Handle output redirection
	if (cmd->out)
	{
		int flags = O_WRONLY | O_CREAT;
		flags |= cmd->append ? O_APPEND : O_TRUNC;
		int fd = open(cmd->out, flags, 0644);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->out, 2);
			ft_putstr_fd(": ", 2);
			perror("");
			ft_free_all(gc);
			exit(1);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("minishell");
			close(fd);
			ft_free_all(gc);
			exit(1);
		}
		close(fd);
	}
}

void parse_redirections(t_cmd_node *cmd, char **args)
{
	int i;
	int j;
	char *last_output;
	int last_output_type;
	int arg_count;

	i = 0;
	arg_count = 0;
	last_output = NULL;
	last_output_type = 0;

	// First pass: count valid arguments and check for syntax errors
	while (args[i])
	{
		if (ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0 ||
			ft_strcmp(args[i], "<") == 0 || ft_strcmp(args[i], "<<") == 0)
		{
			if (!args[i + 1])
			{
				ft_putstr_fd("minishell: syntax error near unexpected token 'newline'\n", 2);
				cmd->err = 1;
				cmd->exec->exit_status = 2;
				return;
			}
			i += 2;
		}
		else
		{
			arg_count++;
			i++;
		}
	}

	// Second pass: process redirections and build new argument array
	i = 0;
	j = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0)
		{
			last_output = args[i + 1];
			last_output_type = (ft_strcmp(args[i], ">>") == 0);
			i += 2;
		}
		else if (ft_strcmp(args[i], "<") == 0 || ft_strcmp(args[i], "<<") == 0)
		{
			cmd->in = args[i + 1];
			i += 2;
		}
		else
		{
			args[j] = args[i];
			j++;
			i++;
		}
	}
	args[j] = NULL;

	if (last_output)
	{
		cmd->out = last_output;
		cmd->append = last_output_type;
	}
}
