/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_util_5.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:14:33 by hhussein          #+#    #+#             */
/*   Updated: 2025/06/26 15:04:03 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"

static int	is_redirection_token(char *token)
{
	if (ft_strcmp(token, ">") == 0)
		return (1);
	if (ft_strcmp(token, ">>") == 0)
		return (1);
	if (ft_strcmp(token, "<") == 0)
		return (1);
	if (ft_strcmp(token, "<<") == 0)
		return (1);
	return (0);
}

static int	count_non_redirection_args(char **args)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (args[i])
	{
		if (is_redirection_token(args[i]) && args[i + 1])
			i += 2;
		else
		{
			count++;
			i++;
		}
	}
	return (count);
}

static void	copy_non_redirection_args(char **filtered, char **args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (args[i])
	{
		if (is_redirection_token(args[i]) && args[i + 1])
			i += 2;
		else
		{
			filtered[j] = args[i];
			j++;
			i++;
		}
	}
	filtered[j] = NULL;
}

char	**filter_command_args(t_cmd_node *cmd, char **args)
{
	int		arg_count;
	char	**filtered;

	arg_count = count_non_redirection_args(args);
	filtered = ft_malloc(&cmd->exec->gc, sizeof(char *) * (arg_count + 1));
	if (!filtered)
		return (NULL);
	copy_non_redirection_args(filtered, args);
	return (filtered);
}
