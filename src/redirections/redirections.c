/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 19:55:04 by lyoussef          #+#    #+#             */
/*   Updated: 2025/05/04 17:30:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// heredoc on its own to take care of

#include "minishell.h"

void	parse_redirections(t_cmd_node *cmd, char **args)
{
	char	**processed_args;

	processed_args = args;
	preprocess_attached_redirs(cmd, &processed_args);
	cmd->arr = processed_args;
	process_input_redirection(cmd, processed_args);
	process_output_redirection(cmd, processed_args);
	cmd->arr = filter_command_args(cmd, processed_args);
	print_debug_output(cmd);
}

static void	handle_input_redirection(t_cmd_node *cmd, t_gc *gc)
{
	int	fd;

	fd = open(cmd->in, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->in, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return ;
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

static void	handle_output(t_cmd_node *cmd, t_gc *gc)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (cmd->append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(cmd->out, flags, 0644);
	if (fd < 0)
		display_file_error(cmd->out, NULL, gc);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell");
		close(fd);
		ft_free_all(gc);
		exit(1);
	}
	close(fd);
}

void	handle_redirection(t_cmd_node *cmd, t_gc *gc)
{
	if (cmd->in)
		handle_input_redirection(cmd, gc);
	if (cmd->out)
		handle_output(cmd, gc);
}
