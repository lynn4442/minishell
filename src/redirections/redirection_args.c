/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_args.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 01:15:30 by marvin            #+#    #+#             */
/*   Updated: 2025/06/23 01:15:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"

/* Count non-redirection arguments */
static int	count_non_redirection_args(char **args)
{
	int	i;
	int	arg_count;

	i = 0;
	arg_count = 0;
	while (args[i])
	{
		if ((ft_strcmp(args[i], ">") == 0
				|| ft_strcmp(args[i], ">>") == 0
				|| ft_strcmp(args[i], "<") == 0
				|| ft_strcmp(args[i], "<<") == 0) && args[i + 1])
		{
			i += 2;
		}
		else
		{
			arg_count++;
			i++;
		}
	}
	return (arg_count);
}

/* Create new argument array without redirection operators */
static char	**create_filtered_args(t_cmd_node *cmd, char **args, int arg_count)
{
	char	**new_args;
	int		i;
	int		j;

	new_args = ft_malloc(&cmd->exec->gc, sizeof(char *) * (arg_count + 1));
	if (!new_args)
		return (NULL);
	i = 0;
	j = 0;
	while (args[i])
	{
		if ((ft_strcmp(args[i], ">") == 0
				|| ft_strcmp(args[i], ">>") == 0
				|| ft_strcmp(args[i], "<") == 0
				|| ft_strcmp(args[i], "<<") == 0) && args[i + 1])
		{
			i += 2;
			continue ;
		}
		new_args[j++] = ft_strdup(&cmd->exec->gc, args[i++]);
	}
	new_args[j] = NULL;
	return (new_args);
}

/* Process command arguments and extract redirection operators */
void	process_and_update_args(t_cmd_node *cmd, char **args)
{
	int		arg_count;
	char	**new_args;

	arg_count = count_non_redirection_args(args);
	new_args = create_filtered_args(cmd, args, arg_count);
	if (new_args)
		cmd->arr = new_args;
}
