/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 22:00:00 by marvin            #+#    #+#             */
/*   Updated: 2025/05/24 22:00:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Saves the original file descriptor by duplicating it */
int	save_original_fd(int fd_to_save, int *original_fd)
{
	*original_fd = dup(fd_to_save);
	if (*original_fd == -1)
	{
		perror("minishell");
		return (-1);
	}
	return (0);
}

/* Redirects a file descriptor to a new target, handling errors and cleanup */
int	redirect_fd(int new_fd, int target_fd, int original_fd)
{
	if (dup2(new_fd, target_fd) == -1)
	{
		perror("minishell");
		close(new_fd);
		close(original_fd);
		return (-1);
	}
	return (0);
}

/* Generic function to restore any file descriptor to its original state */
int	restore_fd_redirection(int original_fd, int target_fd)
{
	if (original_fd != -1)
	{
		if (dup2(original_fd, target_fd) == -1)
		{
			perror("minishell");
			close(original_fd);
			return (-1);
		}
		close(original_fd);
	}
	return (0);
}
