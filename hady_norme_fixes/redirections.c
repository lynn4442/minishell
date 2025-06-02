/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 19:55:04 by lyoussef          #+#    #+#             */
/*   Updated: 2025/05/04 17:30:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// heredoc on its own to take care of

# include "minishell.h"

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
	int	i;

	i = 0;
	if (!str || !*str)
		return 0;
	len = ft_strlen(str);

	if (ft_strcmp(str, ">") == 0 ||// Handle simple cases first
		ft_strcmp(str, ">>") == 0 ||
		ft_strcmp(str, "<") == 0 ||
		ft_strcmp(str, "<<") == 0)
		return (1);

	while (i < len)// Check for redirection characters at the start or middle
	{
		if (str[i] == '>' || str[i] == '<')
			return 1;
		i++;
	}
	return (0);
}

/* Process attached redirections like 'ls>test' or '>file' */
// static char **process_attached_redirections(char **args, t_gc *gc, int *position)
// {
// 	char *arg = args[*position];
// 	int len = ft_strlen(arg);
// 	int redir_pos = -1;
// 	char redir_type = 0;
// 	char *cmd_part = NULL;
// 	char *filename_part = NULL;
// 	char *redir_token = NULL;

// 	ft_putstr_fd("DEBUG - Processing attached redirection: [", 2);
// 	ft_putstr_fd(arg, 2);
// 	ft_putstr_fd("]\n", 2);
// 	if (len <= 1 || ft_strcmp(arg, ">") == 0 || ft_strcmp(arg, "<") == 0 || // No need to process if too short or is a standalone redirect
// 		ft_strcmp(arg, ">>") == 0 || ft_strcmp(arg, "<<") == 0)
// 		return args;
// 	for (int i = 0; i < len; i++)// Find the position of redirection character
// 	{
// 		if (arg[i] == '>' || arg[i] == '<')
// 		{
// 			redir_pos = i;
// 			redir_type = arg[i];
// 			break;
// 		}
// 	}
// 	if (redir_pos == -1)
// 		return args; // No redirection found
// 	int count = 0;// Count existing args
// 	while (args[count])
// 		count++;
// 	// Now handle different cases
// 	// Case 1: Command followed by redirection (e.g. "ls>")
// 	if (redir_pos == len - 1)
// 	{
// 		ft_putstr_fd("DEBUG - Case 1: Command with redirection at end\n", 2);
// 		cmd_part = ft_strndup(gc, arg, len - 1);
// 		redir_token = ft_strndup(gc, &arg[len - 1], 1);

// 		// Create new args array with space for additional token
// 		char **new_args = ft_malloc(gc, sizeof(char *) * (count + 2));
// 		if (!new_args || !cmd_part || !redir_token)
// 			return args;

// 		// Copy before current
// 		int i;
// 		for (i = 0; i < *position; i++)
// 			new_args[i] = args[i];

// 		// Add the split parts
// 		new_args[i++] = cmd_part;
// 		new_args[i++] = redir_token;

// 		// Copy remaining
// 		int j = *position + 1;
// 		while (args[j])
// 			new_args[i++] = args[j++];

// 		new_args[i] = NULL;

// 		// Don't increment position to process the newly added redirection token
// 		return new_args;
// 	}
// 	// Case 2: Redirection at start (e.g. ">file")
// 	else if (redir_pos == 0)
// 	{
// 		ft_putstr_fd("DEBUG - Case 2: Redirection at start\n", 2);
// 		// Check for double redirection
// 		if (len > 1 && arg[1] == redir_type)
// 		{
// 			// Double redirection (>> or <<)
// 			redir_token = ft_strndup(gc, arg, 2);
// 			filename_part = ft_strdup(gc, arg + 2);
// 		}
// 		else
// 		{
// 			// Single redirection
// 			redir_token = ft_strndup(gc, arg, 1);
// 			filename_part = ft_strdup(gc, arg + 1);
// 		}

// 		// Create new args array
// 		char **new_args = ft_malloc(gc, sizeof(char *) * (count + 2)); // One extra for NULL
// 		if (!new_args || !redir_token || !filename_part)
// 			return args;

// 		// Copy before current
// 		int i;
// 		for (i = 0; i < *position; i++)
// 			new_args[i] = args[i];

// 		// Add the split parts
// 		new_args[i++] = redir_token;
// 		new_args[i++] = filename_part;

// 		// Copy remaining
// 		int j = *position + 1;
// 		while (args[j])
// 			new_args[i++] = args[j++];

// 		new_args[i] = NULL;

// 		// Don't increment position to process the newly added redirection token
// 		return new_args;
// 	}

// 	// Case 3: Redirection in the middle (e.g. "ls>file")
// 	else
// 	{
// 		ft_putstr_fd("DEBUG - Case 3: Redirection in middle\n", 2);
// 		// Get command part
// 		cmd_part = ft_strndup(gc, arg, redir_pos);

// 		// Check for double redirection
// 		if (redir_pos + 1 < len && arg[redir_pos + 1] == redir_type)
// 		{
// 			// Double redirection
// 			redir_token = ft_strndup(gc, arg + redir_pos, 2);
// 			filename_part = ft_strdup(gc, arg + redir_pos + 2);
// 		}
// 		else
// 		{
// 			// Single redirection
// 			redir_token = ft_strndup(gc, arg + redir_pos, 1);
// 			filename_part = ft_strdup(gc, arg + redir_pos + 1);
// 		}

// 		// Create new args array
// 		char **new_args = ft_malloc(gc, sizeof(char *) * (count + 3)); // Two extras plus NULL
// 		if (!new_args || !cmd_part || !redir_token || !filename_part)
// 			return args;

// 		// Copy before current
// 		int i;
// 		for (i = 0; i < *position; i++)
// 			new_args[i] = args[i];

// 		// Add the split parts
// 		new_args[i++] = cmd_part;
// 		new_args[i++] = redir_token;
// 		new_args[i++] = filename_part;

// 		// Copy remaining
// 		int j = *position + 1;
// 		while (args[j])
// 			new_args[i++] = args[j++];

// 		new_args[i] = NULL;

// 		ft_putstr_fd("DEBUG - Split into: [", 2);
// 		ft_putstr_fd(cmd_part, 2);
// 		ft_putstr_fd("] [", 2);
// 		ft_putstr_fd(redir_token, 2);
// 		ft_putstr_fd("] [", 2);
// 		ft_putstr_fd(filename_part, 2);
// 		ft_putstr_fd("]\n", 2);

// 		// For attached redirections, create the file immediately to handle cases like ls>file
// 		// This ensures all redirection files are created regardless of parsing order
// 		if (redir_type == '>' && filename_part && *filename_part)
// 		{
// 			int flags = O_WRONLY | O_CREAT;
// 			int is_append = (redir_pos + 1 < len && arg[redir_pos + 1] == '>');
// 			flags |= is_append ? O_APPEND : O_TRUNC;

// 			ft_putstr_fd("DEBUG - Creating file for attached redirection: ", 2);
// 			ft_putstr_fd(filename_part, 2);
// 			ft_putstr_fd("\n", 2);

// 			int fd = open(filename_part, flags, 0644);
// 			if (fd >= 0)
// 				close(fd);
// 		}

// 		return new_args;
// 	}
// }

static int	find_redir_pos(const char *arg, char *redir_type)
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

static void	print_split_debug(char *cmd, char *redir, char *file)
{
	ft_putstr_fd("DEBUG - Split into: [", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd("] [", 2);
	ft_putstr_fd(redir, 2);
	ft_putstr_fd("] [", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd("]\n", 2);
}

static void	create_file_for_redir(char *filename, char *arg, int pos)
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
	ft_putstr_fd("DEBUG - Creating file for attached redirection: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd("\n", 2);
	fd = open(filename, flags, 0644);
	if (fd >= 0)
		close(fd);
}

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
	ft_putstr_fd("DEBUG - Case 1: Command with redirection at end\n", 2);
	tokens[0] = ft_strndup(gc, arg, redir_pos);          // command
	tokens[1] = ft_strndup(gc, &arg[redir_pos], 1);      // redirection symbol
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
	ft_putstr_fd("DEBUG - Case 2: Redirection at start\n", 2);
	if (arg[1] == redir_type)
	{
		tokens[1] = ft_strndup(gc, arg, 2);           // e.g., >>
		tokens[2] = ft_strdup(gc, arg + 2);           // filename
	}
	else
	{
		tokens[1] = ft_strndup(gc, arg, 1);           // >
		tokens[2] = ft_strdup(gc, arg + 1);           // filename
	}
	tokens[0] = NULL;
	tokens[3] = NULL;
	return (tokens);
}
static char	**handle_redir_middle(t_gc *gc, char *arg, int redir_pos, char redir_type)
{
	char	**tokens;

	tokens = ft_malloc(gc, sizeof(char *) * 4);
	if (!tokens)
		return (NULL);
	ft_putstr_fd("DEBUG - Case 3: Redirection in middle\n", 2);
	tokens[0] = ft_strndup(gc, arg, redir_pos); // command
	if (arg[redir_pos + 1] == redir_type)
	{
		tokens[1] = ft_strndup(gc, arg + redir_pos, 2);     // >> or <<
		tokens[2] = ft_strdup(gc, arg + redir_pos + 2);     // filename
	}
	else
	{
		tokens[1] = ft_strndup(gc, arg + redir_pos, 1);     // > or <
		tokens[2] = ft_strdup(gc, arg + redir_pos + 1);     // filename
	}
	tokens[3] = NULL;
	return (tokens);
}

static char	**handle_redir_case(char **args, t_gc *gc, int *pos,
	int redir_pos, char redir_type)
{
	char	*arg;
	char	**tokens;

	arg = args[*pos];
	if (redir_pos == ft_strlen(arg) - 1)
		tokens = handle_redir_end(gc, arg, redir_pos);
	else if (redir_pos == 0)
		tokens = handle_redir_start(gc, arg, redir_type);
	else
		tokens = handle_redir_middle(gc, arg, redir_pos, redir_type);
	if (!tokens)
		return (args);
	if (tokens[0] && tokens[1] && tokens[2])
		print_split_debug(tokens[0], tokens[1], tokens[2]);
	if (redir_type == '>' && tokens[2])
		create_file_for_redir(tokens[2], arg, redir_pos);
	return (build_new_args(args, gc, tokens, pos));
}

char	**process_attached_redirections(char **args, t_gc *gc, int *position)
{
	char	*arg;
	int		len;
	int		redir_pos;
	char	redir_type;

	arg = args[*position];
	ft_putstr_fd("DEBUG - Processing attached redirection: [", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("]\n", 2);
	len = ft_strlen(arg);
	if (len <= 1 || ft_strcmp(arg, ">") == 0 || ft_strcmp(arg, "<") == 0
		|| ft_strcmp(arg, ">>") == 0 || ft_strcmp(arg, "<<") == 0)
		return (args);
	redir_pos = find_redir_pos(arg, &redir_type);
	if (redir_pos == -1)
		return (args);
	return (handle_redir_case(args, gc, position, redir_pos, redir_type));
}
/*
 * Find and extract all redirection files from command arguments
 * This is a helper function to get all the files that are redirection targets
 */
// static char **extract_redirection_files(char **args, int **is_append, int *count, t_gc *gc)
// {
// 	char **files;
// 	int i = 0;
// 	int file_count = 0;

// 	// First, count the number of redirections
// 	while (args[i])
// 	{
// 		if ((ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0) && args[i + 1])
// 			file_count++;
// 		i++;
// 	}

// 	// If no redirections, return NULL
// 	if (file_count == 0)
// 	{
// 		*count = 0;
// 		return NULL;
// 	}

// 	// Allocate memory for files and is_append arrays
// 	files = ft_malloc(gc, sizeof(char *) * (file_count + 1));
// 	*is_append = ft_malloc(gc, sizeof(int) * file_count);
// 	if (!files || !*is_append)
// 	{
// 		*count = 0;
// 		return NULL;
// 	}
// 	// Fill the arrays
// 	i = 0;
// 	file_count = 0;
// 	while (args[i])
// 	{
// 		if (ft_strcmp(args[i], ">") == 0 && args[i + 1])
// 		{
// 			files[file_count] = args[i + 1];
// 			(*is_append)[file_count] = 0;
// 			file_count++;
// 			i += 2;
// 		}
// 		else if (ft_strcmp(args[i], ">>") == 0 && args[i + 1])
// 		{
// 			files[file_count] = args[i + 1];
// 			(*is_append)[file_count] = 1;
// 			file_count++;
// 			i += 2;
// 		}
// 		else
// 		{
// 			i++;
// 		}
// 	}
// 	files[file_count] = NULL;
// 	*count = file_count;
// 	return files;
// }

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

static void	init_arrays(char ***files, int **is_append, int file_count, t_gc *gc)
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

static char	**extract_redirection_files(
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

/* Parse and handle redirections in command arguments */
// void parse_redirections(t_cmd_node *cmd, char **args)
// {
// 	int i;
// 	char **processed_args = NULL;

// 	i = 0;

// 	// First, preprocess args to handle attached redirections
// 	processed_args = args;
// 	while (processed_args[i])
// 	{
// 		if (is_redirection(processed_args[i]) && processed_args[i][0] != '>' && processed_args[i][0] != '<')
// 		{
// 			// This arg contains but doesn't start with a redirection, process it
// 			processed_args = process_attached_redirections(processed_args, &cmd->exec->gc, &i);
// 			continue;
// 		}
// 		else if ((processed_args[i][0] == '>' || processed_args[i][0] == '<') && ft_strlen(processed_args[i]) > 1)
// 		{
// 			// This arg starts with a redirection and has attached text, process it
// 			processed_args = process_attached_redirections(processed_args, &cmd->exec->gc, &i);
// 			continue;
// 		}
// 		i++;
// 	}

// 	// Update the command's arguments with the processed ones
// 	cmd->arr = processed_args;

// 	// Find input redirection (last one wins)
// 	i = 0;
// 	while (processed_args[i])
// 	{
// 		if (ft_strcmp(processed_args[i], "<") == 0 && processed_args[i + 1])
// 		{
// 			// Set the input file (last one will be used)
// 			cmd->in = processed_args[i + 1];
// 			i += 2;
// 		}
// 		else
// 		{
// 			i++;
// 		}
// 	}

// 	// Find and process output redirections
// 	int file_count = 0;
// 	int *is_append = NULL;
// 	char **output_files = extract_redirection_files(processed_args, &is_append, &file_count, &cmd->exec->gc);

// 	if (file_count > 0)
// 	{
// 		// Create all files
// 		for (i = 0; i < file_count; i++)
// 		{
// 			int flags = O_WRONLY | O_CREAT;
// 			flags |= is_append[i] ? O_APPEND : O_TRUNC;

// 			ft_putstr_fd("DEBUG - Creating redirection file: ", 2);
// 			ft_putstr_fd(output_files[i], 2);
// 			ft_putstr_fd("\n", 2);

// 			int fd = open(output_files[i], flags, 0644);
// 			if (fd >= 0)
// 				close(fd);
// 		}

// 		// Set the last file as the command's output file
// 		cmd->out = output_files[file_count - 1];
// 		cmd->append = is_append[file_count - 1];
// 	}

// 	// Filter out redirection operators and their targets from the command arguments
// 	// This creates a new array with just the command and its normal arguments
// 	int arg_count = 0;
// 	i = 0;
// 	while (processed_args[i])
// 	{
// 		if ((ft_strcmp(processed_args[i], ">") == 0 ||
// 			 ft_strcmp(processed_args[i], ">>") == 0 ||
// 			 ft_strcmp(processed_args[i], "<") == 0 ||
// 			 ft_strcmp(processed_args[i], "<<") == 0) && processed_args[i + 1])
// 		{
// 			i += 2; // Skip redirection operator and its target
// 		}
// 		else
// 		{
// 			arg_count++;
// 			i++;
// 		}
// 	}

// 	char **filtered_args = ft_malloc(&cmd->exec->gc, sizeof(char *) * (arg_count + 1));
// 	if (!filtered_args)
// 		return;

// 	i = 0;
// 	int j = 0;
// 	while (processed_args[i])
// 	{
// 		if ((ft_strcmp(processed_args[i], ">") == 0 ||
// 			 ft_strcmp(processed_args[i], ">>") == 0 ||
// 			 ft_strcmp(processed_args[i], "<") == 0 ||
// 			 ft_strcmp(processed_args[i], "<<") == 0) && processed_args[i + 1])
// 		{
// 			i += 2; // Skip redirection operator and its target
// 		}
// 		else
// 		{
// 			filtered_args[j++] = processed_args[i++];
// 		}
// 	}
// 	filtered_args[j] = NULL;

// 	// Replace the command's arguments with the filtered ones
// 	cmd->arr = filtered_args;

// 	ft_putstr_fd("DEBUG - Final command after redirection processing: ", 2);
// 	i = 0;
// 	while (cmd->arr[i])
// 	{
// 		ft_putstr_fd(cmd->arr[i], 2);
// 		ft_putstr_fd(" ", 2);
// 		i++;
// 	}
// 	ft_putstr_fd("\n", 2);

// 	if (cmd->in)
// 	{
// 		ft_putstr_fd("DEBUG - Input from: ", 2);
// 		ft_putstr_fd(cmd->in, 2);
// 		ft_putstr_fd("\n", 2);
// 	}

// 	if (cmd->out)
// 	{
// 		ft_putstr_fd("DEBUG - Output to: ", 2);
// 		ft_putstr_fd(cmd->out, 2);
// 		ft_putstr_fd(cmd->append ? " (append)" : " (truncate)", 2);
// 		ft_putstr_fd("\n", 2);
// 	}
// }
static int is_attached_redir(char *arg)
{
	if (is_redirection(arg) && arg[0] != '>' && arg[0] != '<')
		return (1);
	if ((arg[0] == '>' || arg[0] == '<') && ft_strlen(arg) > 1)
		return (1);
	return (0);
}

static void preprocess_attached_redirs(t_cmd_node *cmd, char ***args)
{
	int i;
	char **processed;
	i = 0;
	processed = *args;
	while (processed[i])
	{
		if (is_attached_redir(processed[i]))
		{
			processed = process_attached_redirections(processed, &cmd->exec->gc, &i);
			continue;
		}
		i++;
	}
	*args = processed;
}

static void process_input_redirection(t_cmd_node *cmd, char **args)
{
	int i;
	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "<") == 0 && args[i + 1])
		{
			cmd->in = args[i + 1];
			i += 2;
		}
		else
			i++;
	}
}

static void	process_output_file(t_cmd_node *cmd, char *file, int append)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	ft_putstr_fd("DEBUG - Creating redirection file: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd("\n", 2);
	fd = open(file, flags, 0644);
	if (fd >= 0)
		close(fd);
}

static void	process_output_redirection(t_cmd_node *cmd, char **args)
{
	int		file_count;
	int		*is_append;
	char	**files;
	int		i;

	file_count = 0;
	is_append = NULL;
	files = extract_redirection_files(args, &is_append, &file_count, &cmd->exec->gc);
	if (file_count <= 0)
		return;
	i = 0;
	while (i < file_count)
	{
		process_output_file(cmd, files[i], is_append[i]);
		i++;
	}
	cmd->out = files[file_count - 1];
	cmd->append = is_append[file_count - 1];
}


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

static char	**filter_command_args(t_cmd_node *cmd, char **args)
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


static void	print_debug_output(t_cmd_node *cmd)
{
	int	i;

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
		if (cmd->append)
			ft_putstr_fd(" (append)\n", 2);
		else
			ft_putstr_fd(" (truncate)\n", 2);
	}
}

void	parse_redirections(t_cmd_node *cmd, char **args)
{
	char	**processed_args;

	processed_args = args;
	preprocess_attached_redirs(cmd, &processed_args);
	cmd->arr = processed_args;
	process_input_redirection(cmd, processed_args);
	process_output_redirection(cmd, processed_args);
	cmd->arr = filter_command_args(cmd, processed_args);
	print_debug_output(cmd);
}

/* Handle redirection for command execution */
// void handle_redirection(t_cmd_node *cmd, t_gc *gc)
// {
// 	int fd;

// 	// Handle input redirection
// 	if (cmd->in)
// 	{
// 		fd = open(cmd->in, O_RDONLY);
// 		if (fd < 0)
// 		{
// 			ft_putstr_fd("minishell: ", 2);
// 			ft_putstr_fd(cmd->in, 2);
// 			ft_putstr_fd(": No such file or directory\n", 2);
// 			return;
// 		}

// 		if (dup2(fd, STDIN_FILENO) == -1)
// 		{
// 			perror("minishell");
// 			close(fd);
// 			ft_free_all(gc);
// 			exit(1);
// 		}
// 		close(fd);
// 	}

// 	// Handle output redirection
// 	if (cmd->out)
// 	{
// 		int flags = O_WRONLY | O_CREAT;
// 		flags |= cmd->append ? O_APPEND : O_TRUNC;

// 		fd = open(cmd->out, flags, 0644);
// 		if (fd < 0)
// 			display_file_error(cmd->out, NULL, gc);

// 		if (dup2(fd, STDOUT_FILENO) == -1)
// 		{
// 			perror("minishell");
// 			close(fd);
// 			ft_free_all(gc);
// 			exit(1);
// 		}
// 		close(fd);
// 	}
// }
static void	handle_input_redirection(t_cmd_node *cmd, t_gc *gc)
{
	int	fd;

	fd = open(cmd->in, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->in, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return ;
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
static void	handle_output_redirection(t_cmd_node *cmd, t_gc *gc)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (cmd->append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
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
void	handle_redirection(t_cmd_node *cmd, t_gc *gc)
{
	if (cmd->in)
		handle_input_redirection(cmd, gc);
	if (cmd->out)
		handle_output_redirection(cmd, gc);
}
