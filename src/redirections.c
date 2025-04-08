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
			ft_putstr_fd(": No such file or directory\n", 2);
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
	ft_input_redirection(cmd, gc);
	ft_output_append(cmd, gc);
	ft_output_truncate(cmd, gc);
}

void parse_redirections(t_cmd_node *cmd, char **args)
{
	int i;
	int arg_count;

	i = 0;
	arg_count = 0;

	// First check for syntax errors
	while (args[i])
	{
		if ((ft_strcmp(args[i], ">") == 0 || 
			 ft_strcmp(args[i], ">>") == 0 ||
			 ft_strcmp(args[i], "<") == 0 ||
			 ft_strcmp(args[i], "<<") == 0))
		{
			// Check if there's no argument after redirection operator
			if (!args[i + 1])
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
				ft_putstr_fd(args[i], 2);
				ft_putstr_fd("'\n", 2);
				cmd->err = 1;
				cmd->exec->exit_status = 2;
				return;
			}
		}
		i++;
	}

	// Reset i for normal parsing
	i = 0;
	
	// Process redirections and count valid arguments
	while (args[i])
	{
		if ((ft_strcmp(args[i], ">") == 0 || 
			 ft_strcmp(args[i], ">>") == 0 ||
			 ft_strcmp(args[i], "<") == 0) && args[i + 1])
		{
			if (ft_strcmp(args[i], ">") == 0)
			{
				// Just store the last redirection in cmd->out
				cmd->out = args[i + 1];
				cmd->append = 0;
			}
			else if (ft_strcmp(args[i], ">>") == 0)
			{
				// Just store the last redirection in cmd->out
				cmd->out = args[i + 1];
				cmd->append = 1;
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
}
