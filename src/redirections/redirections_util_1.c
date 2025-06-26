/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_util_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:13:20 by hhussein          #+#    #+#             */
/*   Updated: 2025/06/26 15:04:24 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"
#include "../utils/utils.h"

void	display_file_error(const char *file, const char *message, t_gc *gc)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	if (message)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(message, 2);
	}
	ft_putstr_fd("\n", 2);
	ft_free_all(gc);
	exit(1);
}

int	is_redirection(const char *str)
{
	int	len;
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	len = ft_strlen(str);
	if (ft_strcmp(str, ">") == 0
		|| ft_strcmp(str, ">>") == 0
		|| ft_strcmp(str, "<") == 0
		|| ft_strcmp(str, "<<") == 0)
		return (1);
	while (i < len)
	{
		if (str[i] == '>' || str[i] == '<')
			return (1);
		i++;
	}
	return (0);
}

int	find_redir_pos(const char *arg, char *redir_type)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '>' || arg[i] == '<')
		{
			*redir_type = arg[i];
			return (i);
		}
		i++;
	}
	return (-1);
}

void	create_file_for_redir(char *filename, char *arg, int pos)
{
	int	flags;
	int	fd;

	if (!filename || !*filename)
		return ;
	flags = O_WRONLY | O_CREAT;
	if (arg[pos + 1] == '>')
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd >= 0)
		close(fd);
}
