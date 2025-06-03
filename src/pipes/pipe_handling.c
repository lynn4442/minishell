/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhussein <hhussein@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/03 22:08:49 by hhussein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipes.h"

static int	init_pipes_and_env(t_exec *exec, int pipe_count,
	int pipes[][2], char ***env_array)
{
	if (!create_all_pipes(pipe_count, pipes))
		return (0);
	*env_array = convert_env_to_array(exec, &exec->gc);
	if (!*env_array)
	{
		ft_putstr_fd("minishell: environment conversion failed\n", 2);
		close_all_pipes(pipe_count, pipes);
		return (0);
	}
	return (1);
}

void	execute_with_pipes(t_exec *exec, t_cmd_node *cmd_list)
{
	t_r_variables	var;
	char			**env_array;

	var.cmd_count = count_pipeline_commands(cmd_list);
	if (var.cmd_count <= 0)
		return ;
	if (is_debug_enabled(exec))
		debug_pipeline_commands(exec, cmd_list, var.cmd_count);
	if (var.cmd_count == 1)
	{
		execute_command_supreme(exec, cmd_list);
		return ;
	}
	var.pipe_count = var.cmd_count - 1;
	if (!init_pipes_and_env(exec, var.pipe_count, var.pipes, &env_array))
		return ;
	fork_and_exec_commands(exec, cmd_list, &var, env_array);
	close_all_pipes(var.pipe_count, var.pipes);
	exec->exit_status = wait_for_all_commands(exec, var.pids, var.cmd_count);
	if (is_debug_enabled(exec))
		ft_putstr_fd("minishell: pipeline execution complete\n", 2);
}

static char	*join_command_parts(t_exec *exec, char **parts)
{
	char	*cmd_str;
	char	*tmp;
	int		j;

	cmd_str = NULL;
	j = -1;
	while (parts[++j])
	{
		if (!cmd_str)
			cmd_str = ft_strdup(&exec->gc, parts[j]);
		else
		{
			tmp = ft_strjoin(cmd_str, " ", &exec->gc);
			cmd_str = ft_strjoin(tmp, parts[j], &exec->gc);
		}
	}
	return (cmd_str);
}

static char	*append_command_with_pipe(t_exec *exec, char *input, char *cmd_str)
{
	char	*tmp;

	tmp = ft_strjoin(input, " | ", &exec->gc);
	return (ft_strjoin(tmp, cmd_str, &exec->gc));
}

void	execute_pipe(t_exec *exec, char ***commands, int cmd_count)
{
	char		*input;
	t_cmd_node	*pipe_cmds;
	int			i;
	char		*cmd_str;

	input = NULL;
	pipe_cmds = NULL;
	i = -1;
	while (++i < cmd_count)
	{
		cmd_str = join_command_parts(exec, commands[i]);
		if (!input)
			input = cmd_str;
		else
			input = append_command_with_pipe(exec, input, cmd_str);
	}
	if (input)
		pipe_cmds = parse_piped_commands(input, exec);
	if (pipe_cmds)
		execute_with_pipes(exec, pipe_cmds);
}
