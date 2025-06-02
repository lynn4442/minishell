/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_util1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhussein <hhussein@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 20:52:33 by hhussein          #+#    #+#             */
/*   Updated: 2025/06/02 21:20:52 by hhussein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_parts(char **parts)
{
	int	count;

	count = 0;
	while (parts && parts[count])
		count++;
	return (count);
}

static char	***allocate_commands(int count, t_exec *exec)
{
	char	***commands;

	commands = ft_malloc(&exec->gc, (count + 1) * sizeof(char **));
	return (commands);
}

static char	**process_command_part(char *part, t_exec *exec)
{
	char	*trimmed;
	char	**tokens;

	trimmed = ft_strtrim(part, " \t", &exec->gc);
	if (!trimmed)
		return (NULL);
	if (ft_strlen(trimmed) == 0)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		exec->exit_status = 2;
		return (NULL);
	}
	tokens = split_preserve_quotes(trimmed, &exec->gc);
	return (tokens);
}

char	***split_by_pipe(char *input, t_exec *exec)
{
	char	**parts;
	char	***commands;
	int		cmd_count;
	int		i;

	parts = ft_split(input, '|', &exec->gc);
	if (!parts)
		return (NULL);
	cmd_count = count_parts(parts);
	commands = allocate_commands(cmd_count, exec);
	if (!commands)
		return (NULL);
	i = 0;
	while (i < cmd_count)
	{
		commands[i] = process_command_part(parts[i], exec);
		if (!commands[i])
			return (NULL);
		i++;
	}
	commands[i] = NULL;
	return (commands);
}
