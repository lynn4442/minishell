/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_operators.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 22:00:00 by marvin            #+#    #+#             */
/*   Updated: 2025/05/24 22:00:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Checks if the given argument is a redirection operator (>, >>, <, <<) */
static int	is_redirection_operator(const char *arg)
{
	return (ft_strcmp(arg, ">") == 0
		|| ft_strcmp(arg, ">>") == 0
		|| ft_strcmp(arg, "<") == 0
		|| ft_strcmp(arg, "<<") == 0);
}

/* Counts the number of arguments that are not part of redirection operators */
static int	count_non_redirection_args(char **args)
{
	int	i;
	int	arg_count;

	i = 0;
	arg_count = 0;
	while (args[i])
	{
		if (is_redirection_operator(args[i]) && args[i + 1])
			i += 2;
		else
		{
			arg_count++;
			i++;
		}
	}
	return (arg_count);
}

/* Allocates memory for new argument array with proper size */
static char	**allocate_new_args(t_cmd_node *cmd, int arg_count)
{
	char	**new_args;

	new_args = ft_malloc(&cmd->exec->gc, sizeof(char *) * (arg_count + 1));
	if (!new_args)
		return (NULL);
	return (new_args);
}

/* Copies non-redirection arguments to the new array, 
skipping redirection operators and their targets */
static void	copy_non_red_args(t_cmd_node *cmd, char **args, char **new_args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (args[i])
	{
		if (is_redirection_operator(args[i]) && args[i + 1])
		{
			i += 2;
			continue ;
		}
		new_args[j++] = ft_strdup(&cmd->exec->gc, args[i++]);
	}
	new_args[j] = NULL;
}

/* Processes command arguments by removing 
redirection operators and their targets */
void	process_and_update_args(t_cmd_node *cmd, char **args)
{
	int		arg_count;
	char	**new_args;

	arg_count = count_non_redirection_args(args);
	new_args = allocate_new_args(cmd, arg_count);
	if (!new_args)
		return ;
	copy_non_red_args(cmd, args, new_args);
	cmd->arr = new_args;
}
