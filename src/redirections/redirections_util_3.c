/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_util_3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:13:57 by hhussein          #+#    #+#             */
/*   Updated: 2025/06/25 14:32:15 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"

char	**process_attached_redirections(char **args, t_gc *gc, int *position)
{
	char			*arg;
	int				len;
	t_redir_norm	red;

	arg = args[*position];
	ft_putstr_fd("DEBUG - Processing attached redirection: [", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("]\n", 2);
	len = ft_strlen(arg);
	if (len <= 1 || ft_strcmp(arg, ">") == 0 || ft_strcmp(arg, "<") == 0
		|| ft_strcmp(arg, ">>") == 0 || ft_strcmp(arg, "<<") == 0)
		return (args);
	red.redir_pos = find_redir_pos(arg, &red.redir_type);
	if (red.redir_pos == -1)
		return (args);
	return (handle_redir_case(args, gc, position, &red));
}
/*
 * Find and extract all redirection files from command arguments
 * This is a helper function to get all the files that are redirection targets
 */

static int	count_redirections(char **args)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (args[i])
	{
		if ((ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0)
			&& args[i + 1])
			count++;
		i++;
	}
	return (count);
}

static void	init_arrays(char ***files, int
				**is_append, int file_count, t_gc *gc)
{
	*files = ft_malloc(gc, sizeof(char *) * (file_count + 1));
	*is_append = ft_malloc(gc, sizeof(int) * file_count);
}

static void	fill_redirection_arrays(
	char **args, char **files, int *is_append, int *file_count)
{
	int	i;

	i = 0;
	*file_count = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], ">") == 0 && args[i + 1])
		{
			files[*file_count] = args[i + 1];
			is_append[*file_count] = 0;
			(*file_count)++;
			i += 2;
		}
		else if (ft_strcmp(args[i], ">>") == 0 && args[i + 1])
		{
			files[*file_count] = args[i + 1];
			is_append[*file_count] = 1;
			(*file_count)++;
			i += 2;
		}
		else
			i++;
	}
	files[*file_count] = NULL;
}

char	**extract_redirection_files(
	char **args, int **is_append, int *count, t_gc *gc)
{
	char	**files;
	int		file_count;

	file_count = count_redirections(args);
	if (file_count == 0)
	{
		*count = 0;
		return (NULL);
	}
	init_arrays(&files, is_append, file_count, gc);
	if (!files || !*is_append)
	{
		*count = 0;
		return (NULL);
	}
	fill_redirection_arrays(args, files, *is_append, &file_count);
	*count = file_count;
	return (files);
}
