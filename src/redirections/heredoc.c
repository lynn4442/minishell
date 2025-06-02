/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 09:18:08 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"

/* Create a temporary file for heredoc content
static char *create_heredoc_temp_file(t_exec *exec)
{
    // 1. Create a unique filename using the heredoc_counter from exec structure
    // 2. Format should be "/tmp/heredoc_<counter>"
    // 3. Use ft_malloc for memory allocation
    // 4. Use open() to create the file with O_WRONLY | O_CREAT | O_TRUNC flags
    // 5. Set proper permissions (0644)
    // 6. Increment the heredoc_counter for next use
    // 7. Return the filename or NULL if error
    return (NULL);
}
*/

/* Process and expand variables in heredoc content 
static char *process_heredoc_line(const char *line, t_env_var *env, t_exec *exec)
{
    // Implement this function to:
    // 1. Process the input line to handle variable expansion
    // 2. Use process_quotes() to handle variable expansion
    // 3. Handle environment variables (e.g., $HOME, $USER)
    // 4. Return the processed line or NULL if error
    return (NULL);
}
*/

/* Read heredoc content until delimiter is found 
static int read_heredoc_content(const char *delimiter, t_env_var *env, 
                              t_exec *exec, int fd)
{
    // Implement this function to:
    // 1. Read input lines using readline() until delimiter is found
    // 2. Process each line using process_heredoc_line()
    // 3. Write processed lines to the file descriptor
    // 4. Handle EOF (Ctrl+D) with appropriate warning message
    // 5. Return 0 on success, -1 on error
    return (0);
}
*/

/* Handle heredoc redirection
int handle_heredoc(t_cmd_node *cmd, t_exec *exec)
{
    // Implement this function to:
    // 1. Check if heredoc exists in cmd
    // 2. Create temporary file using create_heredoc_temp_file()
    // 3. Open the file for writing
    // 4. Read and process content using read_heredoc_content()
    // 5. Set cmd->in to the temporary file path
    // 6. Handle all error cases
    // 7. Return 0 on success, -1 on error
    return (0);
}
*/


/* Clean up heredoc temporary files
void cleanup_heredoc_files(t_cmd_node *cmd)
{
    // Implement this function to:
    // 1. Check if cmd->in is a heredoc file (starts with "/tmp/heredoc_")
    // 2. Use unlink() to remove the temporary file
    // 3. Handle any errors silently
} 
*/


/*
	NOTE:
	Hole list lal files li zabattoun kermel l heredoc + chou zabatet fiyoun:
	1. src/minishell.h
		- zeded heredoc_counter aal struct tabaa t_exec 
		- fucntion prototype bi ekhir l header file
		- zeded TOKEN_REDIR_HEREDOC aal enum tabaa l token types

	2. src/init_data.c
		- zeded heredoc_counter aal struct tabaa init_exec()
		- zeded heredoc field in init_cmd_fields()

	3. src/redirection_handling.c
		- zabatet shwey setup_input_redirection_local() to handle heredoc

	4. src/parser.c
		- zedet heredoc_delimiter handling in parse_simple_command()
		- zedet TOKEN_REDIR_HEREDOC token processing
		- zabatet command node creation ta tekhoud l heredoc field kmn 

	5. src/redirections.c
		- zeded heredoc handling in parse_redirections()

	6. src/pipe.c
		- zeded heredoc cleanup
		- zeded pipe execution to handle heredoc files

	7. src/quote_handling.c
		- Modified process_quotes() to support heredoc variable expansion
*/