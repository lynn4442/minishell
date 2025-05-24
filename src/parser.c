/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/05/23 23:00:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Token types for lexical analysis */
typedef enum e_token_type {
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_EOF
} t_token_type;

/* Lexer token structure */
typedef struct s_token {
	t_token_type	type;
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
} t_token;

/* Lexer context */
typedef struct s_lexer {
	char			*input;
	int				position;
	t_gc			*gc;
	int				error;
} t_lexer;

/* Parser context */
typedef struct s_parser {
	t_token			*current_token;
	t_token			*token_list;
	t_exec			*exec;
	int				error;
} t_parser;

/* Initialize the lexer with the input string */
t_lexer *init_lexer(char *input, t_gc *gc)
{
	t_lexer *lexer;

	lexer = ft_malloc(gc, sizeof(t_lexer));
	if (!lexer)
		return (NULL);
	lexer->input = input;
	lexer->position = 0;
	lexer->gc = gc;
	lexer->error = 0;  // Initialize error flag
	return (lexer);
}

/* Check if character is whitespace */
int is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

/* Skip whitespace in the input */
void skip_whitespace(t_lexer *lexer)
{
	while (lexer->input[lexer->position] && 
		   is_whitespace(lexer->input[lexer->position]))
		lexer->position++;
}

/* Create a new token */
t_token *create_token(t_token_type type, char *value, t_gc *gc)
{
	t_token *token;

	token = ft_malloc(gc, sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value ? ft_strdup(gc, value) : NULL;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

/* Add token to the end of the token list */
void add_token(t_token **head, t_token *new_token)
{
	t_token *current;

	if (!*head)
	{
		*head = new_token;
		return;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
	new_token->prev = current;
}

/* Get the next word token */
t_token *get_word_token(t_lexer *lexer, t_gc *gc)
{
	int start;
	char *word;
	int i;
	int j;
	int in_quotes;
	char quote_type;
	char *temp;
	int escaped;

	start = lexer->position;
	i = start;
	in_quotes = 0;
	quote_type = 0;
	escaped = 0;
	
	// Special case for attached redirection operators at the start
	if (lexer->input[i] == '>' || lexer->input[i] == '<')
		return NULL;
	
	// Allocate temporary buffer for word construction
	temp = ft_malloc(gc, ft_strlen(lexer->input) + 1);
	if (!temp)
		return NULL;
	
	j = 0;
	// Process the entire token with quotes
	while (lexer->input[i])
	{
		// Handle quotes first
		if ((lexer->input[i] == '\'' || lexer->input[i] == '"') && !escaped)
		{
			if (!in_quotes)
			{
				in_quotes = 1;
				quote_type = lexer->input[i];
				temp[j++] = lexer->input[i++];  // Keep the opening quote
				continue;
			}
			else if (lexer->input[i] == quote_type)
			{
				in_quotes = 0;
				quote_type = 0;
				temp[j++] = lexer->input[i++];  // Keep the closing quote
				continue;
			}
		}

		// Handle escape character
		if (lexer->input[i] == '\\' && !escaped)
		{
			escaped = 1;
			temp[j++] = lexer->input[i++];
			continue;
		}

		// If current character is escaped, add it as is
		if (escaped)
		{
			temp[j++] = lexer->input[i++];
			escaped = 0;
			continue;
		}
		
		if (!in_quotes && (is_whitespace(lexer->input[i]) || 
			lexer->input[i] == '|' || 
			lexer->input[i] == '>' || 
			lexer->input[i] == '<'))
		{
			break;
		}
		
		temp[j++] = lexer->input[i++];
	}
	temp[j] = '\0';
	
	// Only create token if we have content
	if (j > 0)
	{
		word = ft_strdup(gc, temp);
		lexer->position = i;
		return (create_token(TOKEN_WORD, word, gc));
	}
	
	return NULL;
}

/* Get the next token from the input */
t_token *get_next_token(t_lexer *lexer, t_gc *gc)
{
	char current;
	t_token *word_token;
	
	skip_whitespace(lexer);
	
	if (!lexer->input[lexer->position])
		return (create_token(TOKEN_EOF, NULL, gc));
	
	current = lexer->input[lexer->position];
	
	// Handle pure redirection symbols and pipes
	if (current == '|')
	{
		ft_putstr_fd("DEBUG: Found pipe token\n", 2);
		lexer->position++;
		// Check for double pipe or pipe at start/end
		if (lexer->position == 1 || !lexer->input[lexer->position] || 
			lexer->input[lexer->position] == '|')
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
			lexer->error = 1;
			return (create_token(TOKEN_EOF, NULL, gc));
		}
		return (create_token(TOKEN_PIPE, "|", gc));
	}
	else if (current == '<')
	{
		ft_putstr_fd("DEBUG: Found input redirection\n", 2);
		lexer->position++;
		if (lexer->input[lexer->position] == '<')
		{
			lexer->position++;
			// Check for more < characters (like <<<)
			if (lexer->input[lexer->position] == '<')
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
				ft_putchar_fd('<', 2);
				ft_putchar_fd('<', 2);
				ft_putchar_fd('<', 2);
				ft_putstr_fd("'\n", 2);
				lexer->error = 1;
				return (create_token(TOKEN_EOF, NULL, gc));
			}
			
			// Check if there's no word after heredoc
			skip_whitespace(lexer);
			if (!lexer->input[lexer->position] || lexer->input[lexer->position] == '|' ||
				lexer->input[lexer->position] == '<' || lexer->input[lexer->position] == '>')
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
				lexer->error = 1;
				return (create_token(TOKEN_EOF, NULL, gc));
			}
			
			return (create_token(TOKEN_REDIR_HEREDOC, "<<", gc));
		}
		
		// Check if there's no word after redirection
		skip_whitespace(lexer);
		if (!lexer->input[lexer->position] || lexer->input[lexer->position] == '|' ||
			lexer->input[lexer->position] == '<' || lexer->input[lexer->position] == '>')
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
			lexer->error = 1;
			return (create_token(TOKEN_EOF, NULL, gc));
		}
		
		return (create_token(TOKEN_REDIR_IN, "<", gc));
	}
	else if (current == '>')
	{
		ft_putstr_fd("DEBUG: Found output redirection\n", 2);
		lexer->position++;
		if (lexer->input[lexer->position] == '>')
		{
			lexer->position++;
			ft_putstr_fd("DEBUG: It's an append redirection\n", 2);
			return (create_token(TOKEN_REDIR_APPEND, ">>", gc));
		}
		
		return (create_token(TOKEN_REDIR_OUT, ">", gc));
	}
	else
	{
		// Get a word token
		word_token = get_word_token(lexer, gc);
		if (word_token)
			return word_token;
		
		// If get_word_token returns NULL, try again with the next character
		lexer->position++;
		return get_next_token(lexer, gc);
	}
}

char *sanitize_input(char *input, t_gc *gc)
{
	int len = ft_strlen(input);
	char *sanitized = ft_malloc(gc, len + 1);
	if (!sanitized)
		return input; // Return original if allocation fails
		
	int i = 0;
	int j = 0;
	
	while (input[i])
	{
		sanitized[j++] = input[i++];
	}
	
	sanitized[j] = '\0';
	return sanitized;
}

/* Tokenize the input string */
t_token *tokenize(char *input, t_gc *gc)
{
	t_lexer *lexer;
	t_token *token_list;
	t_token *token;
	char *sanitized_input;

	// First sanitize the input to handle consecutive redirection symbols
	sanitized_input = sanitize_input(input, gc);
	
	lexer = init_lexer(sanitized_input, gc);
	if (!lexer)
		return (NULL);
	
	token_list = NULL;
	
	while (1)
	{
		token = get_next_token(lexer, gc);
		if (!token)
			return (NULL); // Error in tokenization
		
		add_token(&token_list, token);
		
		if (token->type == TOKEN_EOF || lexer->error)  // Check for errors
			break;
	}
	
	if (lexer->error)  // If there was an error, return NULL
		return (NULL);
	
	return (token_list);
}

/*
** Parser functions - responsible for parsing the tokens
*/

/* Initialize the parser with a token list */
t_parser *init_parser(t_token *tokens, t_exec *exec)
{
	t_parser *parser;

	parser = ft_malloc(&exec->gc, sizeof(t_parser));
	if (!parser)
		return (NULL);
	
	parser->current_token = tokens;
	parser->token_list = tokens;
	parser->exec = exec;
	parser->error = 0;
	
	return (parser);
}

/* Advance to the next token */
void advance_token(t_parser *parser)
{
	if (parser->current_token && 
		parser->current_token->type != TOKEN_EOF)
		parser->current_token = parser->current_token->next;
}

/* Parse a simple command */
t_cmd_node *parse_simple_command(t_parser *parser)
{
	t_cmd_node *cmd;
	char **args;
	int arg_count;
	int i;
	char *last_input = NULL;
	char *last_output = NULL;
	int sflag = 0;
	int last_output_is_append = 0;

	// First, count the number of word tokens for this command
	arg_count = 0;
	
	// First pass: Count args and track redirection info
	t_token *current = parser->current_token;
	while (current && current->type != TOKEN_EOF && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD)
		{
			// Only skip words that are redirection targets (the word after the redirection operator)
			if (!(current != parser->current_token && 
				  (current->prev->type == TOKEN_REDIR_IN ||
				   current->prev->type == TOKEN_REDIR_OUT ||
				   current->prev->type == TOKEN_REDIR_APPEND)))
			{
				arg_count++;
			}
		}
		else if ((current->type == TOKEN_REDIR_OUT ||
				 current->type == TOKEN_REDIR_APPEND) && sflag == 0)
		{
			last_output_is_append = (current->type == TOKEN_REDIR_APPEND);
			if (current->next && current->next->type == TOKEN_WORD)
			{
				char *filename = current->next->value;
				// Remove quotes if present
				if (filename[0] == '"' && filename[ft_strlen(filename) - 1] == '"')
				{
					char *unquoted = ft_substr(filename, 1, ft_strlen(filename) - 2);
					if (unquoted)
					{
						filename = ft_strdup(&parser->exec->gc, unquoted);
						free(unquoted);
					}
				}
				last_output = filename;
			}
		}
		else if (current->type == TOKEN_REDIR_IN)
		{
			if (current->next && current->next->type == TOKEN_WORD)
			{
				char *filename = current->next->value;
				// Remove quotes if present
				if (filename[0] == '"' && filename[ft_strlen(filename) - 1] == '"')
				{
					char *unquoted = ft_substr(filename, 1, ft_strlen(filename) - 2);
					if (unquoted)
					{
						filename = ft_strdup(&parser->exec->gc, unquoted);
						free(unquoted);
					}
				}
				last_input = filename;
				
				// Check if input file exists
				if (access(filename, F_OK) == -1)
				{
					ft_putstr_fd("minishell: ", 2);
					ft_putstr_fd(filename, 2);
					ft_putstr_fd(": No such file or directory\n", 2);
					// parser->error = 1;
					// return (NULL);
					sflag = 1;
					current = current->next;
					continue;
				}
			}
		}
		current = current->next;
	}

	// Allocate args array
	args = ft_malloc(&parser->exec->gc, sizeof(char *) * (arg_count + 1));
	if (!args)
	{
		parser->error = 1;
		return (NULL);
	}
	
	// Fill the args array
	i = 0;
	current = parser->current_token;
	while (current && current->type != TOKEN_EOF && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD)
		{
			// Only skip words that are redirection targets (the word after the redirection operator)
			if (!(current != parser->current_token && 
				  (current->prev->type == TOKEN_REDIR_IN ||
				   current->prev->type == TOKEN_REDIR_OUT ||
				   current->prev->type == TOKEN_REDIR_APPEND)))
			{
				args[i++] = ft_strdup(&parser->exec->gc, current->value);
			}
		}
		current = current->next;
	}
	args[i] = NULL;

	// Advance parser position to end of command
	while (parser->current_token && 
		   parser->current_token->type != TOKEN_EOF &&
		   parser->current_token->type != TOKEN_PIPE)
	{
		advance_token(parser);
	}
	
	// Create command node
	cmd = create_cmd_node(parser->exec, args);
	if (!cmd)
	{
		parser->error = 1;
		return (NULL);
	}
	
	// Set input and output redirections
	if (last_input)
	{
		cmd->in = ft_strdup(&parser->exec->gc, last_input);
	}
	
	if (last_output)
	{
		cmd->out = ft_strdup(&parser->exec->gc, last_output);
		cmd->append = last_output_is_append;
	}
	
	return (cmd);
}

/* Parse a pipeline */
t_cmd_node *parse_pipeline(t_parser *parser)
{
	t_cmd_node *first_cmd;
	t_cmd_node *current_cmd;
	t_cmd_node *next_cmd;
	int pipe_count = 0;
	t_token *start_token;
	int		sflag = 0;

	ft_putstr_fd("\n=== Pipeline Output File Creation Pass ===\n", 2);
	
	// First, create all output files in the pipeline
	start_token = parser->current_token;
	while (parser->current_token && 
		   parser->current_token->type != TOKEN_EOF
			&& sflag == 0
		)
	{
		ft_putstr_fd("DEBUG: Pipeline token type: ", 2);
		ft_putnbr_fd(parser->current_token->type, 2);
		ft_putstr_fd("\n", 2);
		
		if (parser->current_token->type == TOKEN_REDIR_OUT ||
			parser->current_token->type == TOKEN_REDIR_APPEND)
		{
			int is_append = (parser->current_token->type == TOKEN_REDIR_APPEND);
			advance_token(parser);
			if (parser->current_token && 
				parser->current_token->type == TOKEN_WORD)
			{
				char *filename = parser->current_token->value;
				ft_putstr_fd("DEBUG: Found pipeline output file: ", 2);
				ft_putstr_fd(filename, 2);
				ft_putstr_fd("\n", 2);
				
				// Remove quotes if present
				if (filename[0] == '"' && filename[ft_strlen(filename) - 1] == '"')
				{
					char *unquoted = ft_substr(filename, 1, ft_strlen(filename) - 2);
					if (unquoted)
					{
						filename = ft_strdup(&parser->exec->gc, unquoted);
						free(unquoted);
					}
				}
				
				ft_putstr_fd("DEBUG: Creating pipeline output file: ", 2);
				ft_putstr_fd(filename, 2);
				ft_putstr_fd("\n", 2);
				
				int fd = open(filename, 
							O_WRONLY | O_CREAT | (is_append ? O_APPEND : O_TRUNC),
							0644);
				if (fd >= 0)
				{
					ft_putstr_fd("DEBUG: Successfully created pipeline output file\n", 2);
					close(fd);
				}
				else
				{
					ft_putstr_fd("DEBUG: Failed to create pipeline output file: ", 2);
					perror(filename);
				}
			}
		}
		else if (parser->current_token->type == TOKEN_REDIR_IN)
		{
			int fd = open(parser->current_token->value, O_RDONLY);
			if (fd < 0)
			{
				// ft_putstr_fd("NO SUCH FILE OR DIRECTORY", 2);
				sflag = 1;
			}
		}
		advance_token(parser);
	}
	
	// Reset position to start parsing commands
	parser->current_token = start_token;

	// Parse the first command
	first_cmd = parse_simple_command(parser);
	if (!first_cmd || parser->error)
		return (NULL);
	
	current_cmd = first_cmd;
	
	// Continue parsing commands for each pipe token
	while (parser->current_token && 
		   parser->current_token->type == TOKEN_PIPE)
	{
		pipe_count++;
		advance_token(parser); // Skip the pipe token
		
		// Set the current command type to PIPE
		current_cmd->type = PIPE;
		
		// Parse the next command in the pipeline
		next_cmd = parse_simple_command(parser);
		if (parser->error == 1)
		{
			// parser->error = 1;
			return (first_cmd);
		}
		
		// Link the commands
		current_cmd->next = next_cmd;
		current_cmd = next_cmd;
	}
	
	return (first_cmd);
}

/* Main parsing function */
t_cmd_node *parse_input(char *input, t_exec *exec)
{
	t_token *tokens;
	t_parser *parser;
	t_cmd_node *cmd_list;

	// Check for empty input
	if (!input || input[0] == '\0')
		return (NULL);
	
	// Check for unclosed quotes
	if (!check_quotes(input))
	{
		ft_putstr_fd("minishell: syntax error: unclosed quotes\n", 2);
		exec->exit_status = 2;
		return (NULL);
	}
	
	// Tokenize the input
	tokens = tokenize(input, &exec->gc);
	if (!tokens)
		return (NULL);
	
	// Initialize the parser
	parser = init_parser(tokens, exec);
	if (!parser)
		return (NULL);
	
	// Parse the command(s)
	cmd_list = parse_pipeline(parser);
	
	// Only show syntax error if there's no specific error already set
	if (parser->error && exec->exit_status == 0 && !parser->current_token)
	{
		ft_putstr_fd("minishell: syntax error\n", 2);
		exec->exit_status = 2;
	}
	
	return (cmd_list);
}

/* Check for syntax errors in command structure */
int check_syntax(t_cmd_node *cmd_list, t_exec *exec)
{
	t_cmd_node *current;
	
	if (!cmd_list)
		return (0);
	
	current = cmd_list;
	
	while (current)
	{
		// Check for empty commands, but allow redirections without arguments
		if (!current->arr)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
			exec->exit_status = 2;
			return (1);
		}
		
		// The original code incorrectly checked for any command with PIPE type
		// that doesn't have a next command OR has a next with type PIPE.
		// In a valid multi-pipe scenario, intermediate commands should have type PIPE.
		// We only need to check that a PIPE type command has a next command.
		if (current->type == PIPE && !current->next)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
			exec->exit_status = 2;
			return (1);
		}
		
		current = current->next;
	}
	
	return (0);
}

/* Parse input containing multiple commands separated by pipes */
t_cmd_node *parse_piped_commands(char *input, t_exec *exec)
{
    return parse_input(input, exec);
}

// Add debug function at the top after includes
void debug_print_command(t_cmd_node *cmd, int level)
{
	int	i;

	if (!cmd)
		return;
	
	// Print indentation based on level
	for (i = 0; i < level; i++)
		ft_putstr_fd("  ", 2);
	
	// Print command type
	ft_putstr_fd("Command Type: ", 2);
	if (cmd->type == PIPE)
		ft_putstr_fd("PIPE\n", 2);
	else
		ft_putstr_fd("SIMPLE\n", 2);
	
	// Print arguments
	for (i = 0; i < level; i++)
		ft_putstr_fd("  ", 2);
	ft_putstr_fd("Args: ", 2);
	i = 0;
	while (cmd->arr && cmd->arr[i])
	{
		ft_putstr_fd(cmd->arr[i], 2);
		ft_putstr_fd(" ", 2);
		i++;
	}
	ft_putstr_fd("\n", 2);
	
	// Print redirections
	if (cmd->in)
	{
		for (i = 0; i < level; i++)
			ft_putstr_fd("  ", 2);
		ft_putstr_fd("Input: ", 2);
		ft_putstr_fd(cmd->in, 2);
		ft_putstr_fd("\n", 2);
	}
	if (cmd->out)
	{
		for (i = 0; i < level; i++)
			ft_putstr_fd("  ", 2);
		ft_putstr_fd("Output: ", 2);
		ft_putstr_fd(cmd->out, 2);
		ft_putstr_fd(" (", 2);
		ft_putstr_fd(cmd->append ? "append" : "truncate", 2);
		ft_putstr_fd(")\n", 2);
	}
	
	// Recursively print next command
	if (cmd->next)
	{
		for (i = 0; i < level; i++)
			ft_putstr_fd("  ", 2);
		ft_putstr_fd("|\n", 2);
		debug_print_command(cmd->next, level + 1);
	}
}

/* Main parse function for external call */
t_cmd_node *parse_command_line(char *input, t_exec *exec)
{
	t_cmd_node *cmd_list;
	
	// Parse the input
	cmd_list = parse_input(input, exec);
	
	// Only check syntax and show debug output if parsing succeeded
	if (cmd_list)
	{
		// Check for syntax errors
		if (check_syntax(cmd_list, exec))
		{
			exec->cmd_list = NULL;
			return (NULL);
		}
		
		// Add debug output
		ft_putstr_fd("\n=== Parsed Command Structure ===\n", 2);
		debug_print_command(cmd_list, 0);
		ft_putstr_fd("==============================\n\n", 2);
		
		// Store the command list in the exec structure
		exec->cmd_list = cmd_list;
	}
	
	return (cmd_list);
} 