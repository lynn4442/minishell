/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_util_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:13:37 by hhussein          #+#    #+#             */
/*   Updated: 2025/06/26 15:04:40 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"

static char	**build_new_args(char **args, t_gc *gc,
	char **new_tokens, int *pos)
{
	int		i;
	int		j;
	int		count;
	char	**new_args;

	count = 0;
	while (args[count])
		count++;
	new_args = ft_malloc(gc, sizeof(char *) * (count + 4));
	if (!new_args)
		return (args);
	i = -1;
	while (++i < *pos)
		new_args[i] = args[i];
	j = 0;
	while (new_tokens[j])
		new_args[i++] = new_tokens[j++];
	j = *pos + 1;
	while (args[j])
		new_args[i++] = args[j++];
	new_args[i] = NULL;
	return (new_args);
}

static char	**handle_redir_end(t_gc *gc, char *arg, int redir_pos)
{
	char	**tokens;

	tokens = ft_malloc(gc, sizeof(char *) * 4);
	if (!tokens)
		return (NULL);
	tokens[0] = ft_strndup(gc, arg, redir_pos);
	tokens[1] = ft_strndup(gc, &arg[redir_pos], 1);
	tokens[2] = NULL;
	tokens[3] = NULL;
	return (tokens);
}

static char	**handle_redir_start(t_gc *gc, char *arg, char redir_type)
{
	char	**tokens;

	tokens = ft_malloc(gc, sizeof(char *) * 4);
	if (!tokens)
		return (NULL);
	if (arg[1] == redir_type)
	{
		tokens[1] = ft_strndup(gc, arg, 2);
		tokens[2] = ft_strdup(gc, arg + 2);
	}
	else
	{
		tokens[1] = ft_strndup(gc, arg, 1);
		tokens[2] = ft_strdup(gc, arg + 1);
	}
	tokens[0] = NULL;
	tokens[3] = NULL;
	return (tokens);
}

static char	**handle_redir_middle(t_gc *gc, char *arg,
				int redir_pos, char redir_type)
{
	char	**tokens;

	tokens = ft_malloc(gc, sizeof(char *) * 4);
	if (!tokens)
		return (NULL);
	tokens[0] = ft_strndup(gc, arg, redir_pos);
	if (arg[redir_pos + 1] == redir_type)
	{
		tokens[1] = ft_strndup(gc, arg + redir_pos, 2);
		tokens[2] = ft_strdup(gc, arg + redir_pos + 2);
	}
	else
	{
		tokens[1] = ft_strndup(gc, arg + redir_pos, 1);
		tokens[2] = ft_strdup(gc, arg + redir_pos + 1);
	}
	tokens[3] = NULL;
	return (tokens);
}

char	**handle_redir_case(char **args, t_gc *gc, int *pos,
	t_redir_norm *red)
{
	char	*arg;
	char	**tokens;

	arg = args[*pos];
	if (red->redir_pos == (int)ft_strlen(arg) - 1)
		tokens = handle_redir_end(gc, arg, red->redir_pos);
	else if (red->redir_pos == 0)
		tokens = handle_redir_start(gc, arg, red->redir_type);
	else
		tokens = handle_redir_middle(gc, arg, red->redir_pos, red->redir_type);
	if (!tokens)
		return (args);
	if (red->redir_type == '>' && tokens[2])
		create_file_for_redir(tokens[2], arg, red->redir_pos);
	return (build_new_args(args, gc, tokens, pos));
}
