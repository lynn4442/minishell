/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 10:27:30 by marvin            #+#    #+#             */
/*   Updated: 2025/05/24 10:27:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Create a temporary file for heredoc content */
// static char *create_heredoc_temp_file(t_exec *exec)
// {
//     // IMPLEMENTATION:
//     // 1. Create a unique temporary file name using a counter
//     // 2. Allocate memory for the filename using ft_malloc
//     // 3. Create the file using open() with O_WRONLY | O_CREAT | O_TRUNC flags
//     // 4. Close the file descriptor
//     // 5. Return the filename or NULL if any step fails
// }

/* Process and expand variables in heredoc content */
// static char *process_heredoc_line(const char *line, t_env_var *env, t_exec *exec)
// {
//     // IMPLEMENTATION:
//     // 1. Process the input line to handle variable expansion
//     // 2. Use process_quotes() to handle variable expansion
//     // 3. Return the processed line or NULL if processing fails
// }

/* Read heredoc content until delimiter is found */
// static int read_heredoc_content(const char *delimiter, t_env_var *env, 
//                               t_exec *exec, int fd)
// {
//     // IMPLEMENTATION:
//     // 1. Read input lines using readline() until delimiter is found
//     // 2. Process each line using process_heredoc_line()
//     // 3. Write processed lines to the file descriptor using write()
//     // 4. Handle EOF (Ctrl+D) with appropriate warning message
//     // 5. Return 0 on success, -1 on error
// }

/* Handle heredoc redirection */
// int handle_heredoc(t_cmd_node *cmd, t_exec *exec)
// {
//     // IMPLEMENTATION:
//     // 1. Check if heredoc exists in command
//     // 2. Create temporary file using create_heredoc_temp_file()
//     // 3. Open file for writing
//     // 4. Read and process content using read_heredoc_content()
//     // 5. Set command's input file to temporary file
//     // 6. Return 0 on success, -1 on error
// }

/* Clean up heredoc temporary files */
// void cleanup_heredoc_files(t_cmd_node *cmd)
// {
//     // IMPLEMENTATION:
//     // 1. Check if command's input file is a heredoc temporary file
//     // 2. Use unlink() to remove the temporary file
// } 


/*
	NOTE:
	1) minishell.h:
		- zeded prototypes for heredoc functions
		- zeded heredoc fields aal t_cmd_node struct:
			* heredoc: Stores the delimiter
			* ex_heredoc: flag lal for heredoc execution

	2) parser.c:
		- zabatet parse_command ta tekhoud heredoc tokens
		- zeded TOKEN_REDIR_HEREDOC laennon ken already fi for other redirections
		- zabatet l command node creation ta tekhoud heredoc delimiter

	3) redirection_handling.c:
		- zabatet setup_input_redirection_local ta tekhoud heredoc
		- zeded heredoc processing before regular input redirection
		- zeded error handling for heredoc cases

	4) signal_handling.c:
		- zeded signal handling for heredoc input
		- zeded Ctrl+C during heredoc input
		- zeded signal state during heredoc execution
*/