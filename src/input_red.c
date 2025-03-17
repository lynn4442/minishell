/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_red.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 19:55:04 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/10 20:16:18 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

void ft_input_red(const char *filename)
{
	int fd = open(filename, O_RDONLY);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

void execute_ft_input_red(t_cmd_node *cmd)
{
	if (!cmd || !cmd->arr || !cmd->arr[0])
		return;

	pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		if (cmd->in)
			ft_input_red(cmd->in);
		execvp(cmd->arr[0], cmd->arr);
		perror("execvp");
		exit(EXIT_FAILURE);
	}
	wait(NULL);
}

//execvp(command, args) --> bs baatiya l command hiye bet nabbich lahala
//execve(path, args, envp) --> eza aande full control aal execution yaane baarif kelchi aal env variable
