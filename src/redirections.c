/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhussein <hhussein@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 19:55:04 by lyoussef          #+#    #+#             */
/*   Updated: 2025/05/26 18:38:04 by hhussein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// heredoc on its own to take care of

# include "../minishell.h"

/* Display error message for file operations and exit */
static void display_file_error(const char *file, const char *message, t_gc *gc)
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
/* Check if a string is or contains a redirection operator */
static int is_redirection(const char *str)
{
	int len;
	
	if (!str || !*str)
		return 0;
	
	len = ft_strlen(str);
	
	// Handle simple cases first
	if (ft_strcmp(str, ">") == 0 ||
		ft_strcmp(str, ">>") == 0 ||
		ft_strcmp(str, "<") == 0 ||
		ft_strcmp(str, "<<") == 0)
		return 1;
		
	// Check for redirection characters at the start or middle
	for (int i = 0; i < len; i++)
	{
		if (str[i] == '>' || str[i] == '<')
			return 1;
	}
	
	return 0;
}
/* Process attached redirections like 'ls>test' or '>file' */
static char **process_attached_redirections(char **args, t_gc *gc, int *position)
{
	char *arg = args[*position];
	int len = ft_strlen(arg);
	int redir_pos = -1;
	char redir_type = 0;
	char *cmd_part = NULL;
	char *filename_part = NULL;
	char *redir_token = NULL;
	int		i = 0;
	int 	count = 0;
	int 	j = 0;
	ft_putstr_fd("DEBUG - Processing attached redirection: [", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("]\n", 2);
	
	// No need to process if too short or is a standalone redirect
	if (len <= 1 || ft_strcmp(arg, ">") == 0 || ft_strcmp(arg, "<") == 0 ||
		ft_strcmp(arg, ">>") == 0 || ft_strcmp(arg, "<<") == 0)
		return args;
	// Find the position of redirection character
	while (i < len)
	{
		if (arg[i] == '>' || arg[i] == '<')
		{
			redir_pos = i;
			redir_type = arg[i];
			break;
		}
		i++;
	}
	if (redir_pos == -1)
		return args; // No redirection found
	while (args[count])
		count++;
	if (redir_pos == len - 1) // Case 1: Command followed by redirection (e.g. "ls>")
	{
		ft_putstr_fd("DEBUG - Case 1: Command with redirection at end\n", 2);
		cmd_part = ft_strndup(gc, arg, len - 1);
		redir_token = ft_strndup(gc, &arg[len - 1], 1);
		char **new_args = ft_malloc(gc, sizeof(char *) * (count + 2)); // Create new args array with space for additional token
		if (!new_args || !cmd_part || !redir_token)
			return args;
		i = 0;
		while (i < *position)
		{
			new_args[i] = args[i];
			i++;
		}
		new_args[i++] = cmd_part;
		new_args[i++] = redir_token;
		j = *position + 1;
		while (args[j])
			new_args[i++] = args[j++];
		new_args[i] = NULL;
		return new_args;
	}
	else if (redir_pos == 0) // Case 2: Redirection at start (e.g. ">file")
	{
		ft_putstr_fd("DEBUG - Case 2: Redirection at start\n", 2);
		if (len > 1 && arg[1] == redir_type) // Check for double redirection
		{
			redir_token = ft_strndup(gc, arg, 2); // Double redirection (>> or <<)
			filename_part = ft_strdup(gc, arg + 2);
		}
		else
		{
			redir_token = ft_strndup(gc, arg, 1); // Single redirection
			filename_part = ft_strdup(gc, arg + 1);
		}
		char **new_args = ft_malloc(gc, sizeof(char *) * (count + 2)); // One extra for NULL 
		if (!new_args || !redir_token || !filename_part)
			return args;
		i = 0;
		for (i = 0; i < *position; i++)
			new_args[i] = args[i];
		new_args[i++] = redir_token;
		new_args[i++] = filename_part;
		int j = *position + 1;
		while (args[j])
			new_args[i++] = args[j++];	
		new_args[i] = NULL; // Don't increment position to process the newly added redirection token
		return new_args;
	}
	// Case 3: Redirection in the middle (e.g. "ls>file")
	else
	{
		ft_putstr_fd("DEBUG - Case 3: Redirection in middle\n", 2);
		// Get command part
		cmd_part = ft_strndup(gc, arg, redir_pos);
		
		// Check for double redirection
		if (redir_pos + 1 < len && arg[redir_pos + 1] == redir_type)
		{
			// Double redirection
			redir_token = ft_strndup(gc, arg + redir_pos, 2);
			filename_part = ft_strdup(gc, arg + redir_pos + 2);
		}
		else
		{
			// Single redirection
			redir_token = ft_strndup(gc, arg + redir_pos, 1);
			filename_part = ft_strdup(gc, arg + redir_pos + 1);
		}
		
		// Create new args array
		char **new_args = ft_malloc(gc, sizeof(char *) * (count + 3)); // Two extras plus NULL
		if (!new_args || !cmd_part || !redir_token || !filename_part)
			return args;
			
		// Copy before current
		int i;
		for (i = 0; i < *position; i++)
			new_args[i] = args[i];
			
		// Add the split parts
		new_args[i++] = cmd_part;
		new_args[i++] = redir_token;
		new_args[i++] = filename_part;
		
		// Copy remaining
		int j = *position + 1;
		while (args[j])
			new_args[i++] = args[j++];
			
		new_args[i] = NULL;
		
		ft_putstr_fd("DEBUG - Split into: [", 2);
		ft_putstr_fd(cmd_part, 2);
		ft_putstr_fd("] [", 2);
		ft_putstr_fd(redir_token, 2);
		ft_putstr_fd("] [", 2);
		ft_putstr_fd(filename_part, 2);
		ft_putstr_fd("]\n", 2);
		
		// For attached redirections, create the file immediately to handle cases like ls>file
		// This ensures all redirection files are created regardless of parsing order
		if (redir_type == '>' && filename_part && *filename_part)
		{
			int flags = O_WRONLY | O_CREAT;
			int is_append = (redir_pos + 1 < len && arg[redir_pos + 1] == '>');
			flags |= is_append ? O_APPEND : O_TRUNC;
			
			ft_putstr_fd("DEBUG - Creating file for attached redirection: ", 2);
			ft_putstr_fd(filename_part, 2);
			ft_putstr_fd("\n", 2);
			
			int fd = open(filename_part, flags, 0644);
			if (fd >= 0)
				close(fd);
		}
		
		return new_args;
	}
}
/* 
 * Find and extract all redirection files from command arguments
 * This is a helper function to get all the files that are redirection targets
 */
static char **extract_redirection_files(char **args, int **is_append, int *count, t_gc *gc)
{
	char **files;
	int i = 0;
	int file_count = 0;
	
	// First, count the number of redirections
	while (args[i])
	{
		if ((ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0) && args[i + 1])
			file_count++;
		i++;
	}
	
	// If no redirections, return NULL
	if (file_count == 0)
	{
		*count = 0;
		return NULL;
	}
	
	// Allocate memory for files and is_append arrays
	files = ft_malloc(gc, sizeof(char *) * (file_count + 1));
	*is_append = ft_malloc(gc, sizeof(int) * file_count);
	if (!files || !*is_append)
	{
		*count = 0;
		return NULL;
	}
	
	// Fill the arrays
	i = 0;
	file_count = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], ">") == 0 && args[i + 1])
		{
			files[file_count] = args[i + 1];
			(*is_append)[file_count] = 0;
			file_count++;
			i += 2;
		}
		else if (ft_strcmp(args[i], ">>") == 0 && args[i + 1])
		{
			files[file_count] = args[i + 1];
			(*is_append)[file_count] = 1;
			file_count++;
			i += 2;
		}
		else
		{
			i++;
		}
	}
	
	files[file_count] = NULL;
	*count = file_count;
	return files;
}

/* Parse and handle redirections in command arguments */
void parse_redirections(t_cmd_node *cmd, char **args)
{
	int i;
	char **processed_args = NULL;
	
	i = 0;
	
	// First, preprocess args to handle attached redirections
	processed_args = args;
	while (processed_args[i])
	{
		if (is_redirection(processed_args[i]) && processed_args[i][0] != '>' && processed_args[i][0] != '<')
		{
			// This arg contains but doesn't start with a redirection, process it
			processed_args = process_attached_redirections(processed_args, &cmd->exec->gc, &i);
			continue;
		}
		else if ((processed_args[i][0] == '>' || processed_args[i][0] == '<') && ft_strlen(processed_args[i]) > 1)
		{
			// This arg starts with a redirection and has attached text, process it
			processed_args = process_attached_redirections(processed_args, &cmd->exec->gc, &i);
			continue;
		}
		i++;
	}
	
	// Update the command's arguments with the processed ones
	cmd->arr = processed_args;
	
	// Find input redirection (last one wins)
	i = 0;
	while (processed_args[i])
	{
		if (ft_strcmp(processed_args[i], "<") == 0 && processed_args[i + 1])
		{
			// Set the input file (last one will be used)
			cmd->in = processed_args[i + 1];
			i += 2;
		}
		else
		{
			i++;
		}
	}
	
	// Find and process output redirections
	int file_count = 0;
	int *is_append = NULL;
	char **output_files = extract_redirection_files(processed_args, &is_append, &file_count, &cmd->exec->gc);
	
	if (file_count > 0)
	{
		// Create all files
		for (i = 0; i < file_count; i++)
		{
			int flags = O_WRONLY | O_CREAT;
			flags |= is_append[i] ? O_APPEND : O_TRUNC;
			
			ft_putstr_fd("DEBUG - Creating redirection file: ", 2);
			ft_putstr_fd(output_files[i], 2);
			ft_putstr_fd("\n", 2);
			
			int fd = open(output_files[i], flags, 0644);
			if (fd >= 0)
				close(fd);
		}
		
		// Set the last file as the command's output file
		cmd->out = output_files[file_count - 1];
		cmd->append = is_append[file_count - 1];
	}
	
	// Filter out redirection operators and their targets from the command arguments
	// This creates a new array with just the command and its normal arguments
	int arg_count = 0;
	i = 0;
	while (processed_args[i])
	{
		if ((ft_strcmp(processed_args[i], ">") == 0 || 
			 ft_strcmp(processed_args[i], ">>") == 0 ||
			 ft_strcmp(processed_args[i], "<") == 0 ||
			 ft_strcmp(processed_args[i], "<<") == 0) && processed_args[i + 1])
		{
			i += 2; // Skip redirection operator and its target
		}
		else
		{
			arg_count++;
			i++;
		}
	}
	
	char **filtered_args = ft_malloc(&cmd->exec->gc, sizeof(char *) * (arg_count + 1));
	if (!filtered_args)
		return;
		
	i = 0;
	int j = 0;
	while (processed_args[i])
	{
		if ((ft_strcmp(processed_args[i], ">") == 0 || 
			 ft_strcmp(processed_args[i], ">>") == 0 ||
			 ft_strcmp(processed_args[i], "<") == 0 ||
			 ft_strcmp(processed_args[i], "<<") == 0) && processed_args[i + 1])
		{
			i += 2; // Skip redirection operator and its target
		}
		else
		{
			filtered_args[j++] = processed_args[i++];
		}
	}
	filtered_args[j] = NULL;
	
	// Replace the command's arguments with the filtered ones
	cmd->arr = filtered_args;
	
	ft_putstr_fd("DEBUG - Final command after redirection processing: ", 2);
	i = 0;
	while (cmd->arr[i])
	{
		ft_putstr_fd(cmd->arr[i], 2);
		ft_putstr_fd(" ", 2);
		i++;
	}
	ft_putstr_fd("\n", 2);
	
	if (cmd->in)
	{
		ft_putstr_fd("DEBUG - Input from: ", 2);
		ft_putstr_fd(cmd->in, 2);
		ft_putstr_fd("\n", 2);
	}
	
	if (cmd->out)
	{
		ft_putstr_fd("DEBUG - Output to: ", 2);
		ft_putstr_fd(cmd->out, 2);
		ft_putstr_fd(cmd->append ? " (append)" : " (truncate)", 2);
		ft_putstr_fd("\n", 2);
	}
}

/* Handle redirection for command execution */
void handle_redirection(t_cmd_node *cmd, t_gc *gc)
{
	int fd;
	
	// Handle input redirection
	if (cmd->in)
	{
		fd = open(cmd->in, O_RDONLY);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->in, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			return;
		}
		
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("minishell");
			close(fd);
			ft_free_all(gc);
			exit(1);
		}
		close(fd);
	}
	// Handle output redirection
	if (cmd->out)
	{
		int flags = O_WRONLY | O_CREAT;
		flags |= cmd->append ? O_APPEND : O_TRUNC;
		
		fd = open(cmd->out, flags, 0644);
		if (fd < 0)
			display_file_error(cmd->out, NULL, gc);
		
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("minishell");
			close(fd);
			ft_free_all(gc);
			exit(1);
		}
		close(fd);
	}
}
