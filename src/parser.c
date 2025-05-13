/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 10:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/28 10:00:00 by lyoussef         ###   ########.fr       */
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
static t_lexer *init_lexer(char *input, t_gc *gc)
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
static int is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

/* Skip whitespace in the input */
static void skip_whitespace(t_lexer *lexer)
{
	while (lexer->input[lexer->position] && 
		   is_whitespace(lexer->input[lexer->position]))
		lexer->position++;
}

/* Create a new token */
static t_token *create_token(t_token_type type, char *value, t_gc *gc)
{
	t_token *token;

	token = ft_malloc(gc, sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value ? ft_strdup(gc, value) : NULL;
	token->next = NULL;
	return (token);
}

/* Add token to the end of the token list */
static void add_token(t_token **head, t_token *new_token)
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
}

/* 
// Unused function - commented out to avoid compiler warnings
static int handle_quotes(t_lexer *lexer, int *end_pos, char quote_char)
{
	int i;
	int escaped;
	
	i = lexer->position + 1;
	escaped = 0;
	
	while (lexer->input[i])
	{
		// Handle escape sequence inside double quotes
		if (lexer->input[i] == '\\' && quote_char == '"' && !escaped)
		{
			escaped = 1;
			i++;
			continue;
		}
		
		// If previous character was escape, just treat this as normal character
		if (escaped)
		{
			escaped = 0;
			i++;
			continue;
		}
		
		// Found closing quote
		if (lexer->input[i] == quote_char)
		{
			*end_pos = i;
			return (1); // Found closing quote
		}
		i++;
	}
	return (0); // No closing quote found
}
*/

/* Get the next word token */
static t_token *get_word_token(t_lexer *lexer, t_gc *gc)
{
	int start;
	char *word;
	int i;
	int in_quotes = 0;
	char quote_type = 0;

	start = lexer->position;
	i = start;
	
	// Special case for attached redirection operators at the start
	if (lexer->input[i] == '>' || lexer->input[i] == '<')
	{
		// Handle redirections separately in get_next_token
		return NULL;
	}
	
	// Process the entire token with quotes
	while (lexer->input[i])
	{
		// Handle quotes
		if ((lexer->input[i] == '\'' || lexer->input[i] == '"'))
		{
			if (!in_quotes)
			{
				// Start of quoted section
				in_quotes = 1;
				quote_type = lexer->input[i];
			}
			else if (lexer->input[i] == quote_type)
			{
				// End of quoted section
				in_quotes = 0;
				quote_type = 0;
			}
			// Otherwise it's a different quote inside quotes, just continue
			i++;
			continue;
		}
		
		// Inside quotes, continue collecting characters
		if (in_quotes)
		{
			i++;
			continue;
		}
		
		// Outside quotes, check for token delimiters
		if (is_whitespace(lexer->input[i]) || 
			lexer->input[i] == '|' || 
			lexer->input[i] == '>' || 
			lexer->input[i] == '<')
		{
			break;
		}
		
		// Regular character
		i++;
	}
	
	// Extract the word - only if we have content
	if (i > start)
	{
		word = ft_strndup(gc, lexer->input + start, i - start);
		lexer->position = i;
		
		// Check for redirection symbols in the word - but allow any number of them
		if (word)
		{
			return (create_token(TOKEN_WORD, word, gc));
		}
		
		return (create_token(TOKEN_WORD, word, gc));
	}
	
	// No content, return NULL so get_next_token can handle the next character
	return NULL;
}

/* Get the next token from the input */
static t_token *get_next_token(t_lexer *lexer, t_gc *gc)
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
		lexer->position++;
		return (create_token(TOKEN_PIPE, "|", gc));
	}
	else if (current == '<')
	{
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
		lexer->position++;
		if (lexer->input[lexer->position] == '>')
		{
			lexer->position++;
			// Check for more > characters (like >>>)
			if (lexer->input[lexer->position] == '>')
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
				ft_putchar_fd('>', 2);
				ft_putchar_fd('>', 2);
				ft_putchar_fd('>', 2);
				ft_putstr_fd("'\n", 2);
				lexer->error = 1;
				return (create_token(TOKEN_EOF, NULL, gc));
			}
			
			// Check if there's no word after append
			skip_whitespace(lexer);
			if (!lexer->input[lexer->position] || lexer->input[lexer->position] == '|' ||
				lexer->input[lexer->position] == '<' || lexer->input[lexer->position] == '>')
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
				lexer->error = 1;
				return (create_token(TOKEN_EOF, NULL, gc));
			}
			
			return (create_token(TOKEN_REDIR_APPEND, ">>", gc));
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

/* Sanitize input by normalizing multiple consecutive redirection symbols */
static char *sanitize_input(char *input, t_gc *gc)
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
static t_token *tokenize(char *input, t_gc *gc)
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
static t_parser *init_parser(t_token *tokens, t_exec *exec)
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
static void advance_token(t_parser *parser)
{
	if (parser->current_token && 
		parser->current_token->type != TOKEN_EOF)
		parser->current_token = parser->current_token->next;
}

/* Parse a simple command */
static t_cmd_node *parse_simple_command(t_parser *parser)
{
	t_cmd_node *cmd;
	char **args;
	int arg_count;
	int i;
	int sflag = 0;
	t_token *start_token;
	char *last_input = NULL;
	char *last_output = NULL;
	int last_output_is_append = 0;

	// First, count the number of word tokens for this command
	arg_count = 0;
	start_token = parser->current_token;
	
	while (parser->current_token && 
		   parser->current_token->type != TOKEN_PIPE && 
		   parser->current_token->type != TOKEN_EOF)
	{
		// Skip redirection and its target
		if (parser->current_token->type == TOKEN_REDIR_IN || 
			parser->current_token->type == TOKEN_REDIR_OUT || 
			parser->current_token->type == TOKEN_REDIR_APPEND || 
			parser->current_token->type == TOKEN_REDIR_HEREDOC)
		{
			advance_token(parser);
			if (parser->current_token && 
				parser->current_token->type == TOKEN_WORD)
				advance_token(parser);
			continue;
		}
		
		if (parser->current_token->type == TOKEN_WORD)
			arg_count++;
		
		advance_token(parser);
	}
	
	// Allocate args array
	args = ft_malloc(&parser->exec->gc, sizeof(char *) * (arg_count + 1));
	if (!args)
	{
		parser->error = 1;
		return (NULL);
	}
	
	// Reset parser position and fill the args array
	i = 0;
	parser->current_token = start_token;
	
	while (parser->current_token && 
		   parser->current_token->type != TOKEN_PIPE && 
		   parser->current_token->type != TOKEN_EOF)
	{
		// Track redirections but exclude them from command arguments
		if (parser->current_token->type == TOKEN_REDIR_IN)
		{
			advance_token(parser);
			if (parser->current_token && 
				parser->current_token->type == TOKEN_WORD)
			{
				last_input = parser->current_token->value;
				advance_token(parser);
			}
			continue;
		}
		else if (parser->current_token->type == TOKEN_REDIR_OUT)
		{
			advance_token(parser);
			if (parser->current_token && 
				parser->current_token->type == TOKEN_WORD)
			{
				last_output = parser->current_token->value;
				last_output_is_append = 0;
				advance_token(parser);
			}
			continue;
		}
		else if (parser->current_token->type == TOKEN_REDIR_APPEND)
		{
			advance_token(parser);
			if (parser->current_token && 
				parser->current_token->type == TOKEN_WORD)
			{
				last_output = parser->current_token->value;
				last_output_is_append = 1;
				advance_token(parser);
			}
			continue;
		}
		else if (parser->current_token->type == TOKEN_REDIR_HEREDOC)
		{
			advance_token(parser);
			if (parser->current_token && 
				parser->current_token->type == TOKEN_WORD)
				advance_token(parser);
			continue;
		}
		
		if (parser->current_token->type == TOKEN_WORD)
			args[i++] = ft_strdup(&parser->exec->gc, parser->current_token->value);
		
		advance_token(parser);
	}
	args[i] = NULL;
	
	// Create command node
	cmd = create_cmd_node(parser->exec, args);
	if (!cmd)
	{
		parser->error = 1;
		return (NULL);
	}
	
	// Set input and output redirections
	if (last_input)
		cmd->in = ft_strdup(&parser->exec->gc, last_input);
	
	if (last_output)
	{
		cmd->out = ft_strdup(&parser->exec->gc, last_output);
		cmd->append = last_output_is_append;
		
		// Create all output files in the chain
		parser->current_token = start_token;
		
		while (parser->current_token && 
		       parser->current_token->type != TOKEN_PIPE && 
		       parser->current_token->type != TOKEN_EOF  &&
		   sflag == 0)
		{
			if (parser->current_token->type == TOKEN_REDIR_OUT || 
			    parser->current_token->type == TOKEN_REDIR_APPEND)
			{
				int is_append = (parser->current_token->type == TOKEN_REDIR_APPEND);
				advance_token(parser);
				
				if (parser->current_token && 
				    parser->current_token->type == TOKEN_WORD)
				{
					// Create the file with appropriate flags
					int flags = O_WRONLY | O_CREAT;
					flags |= is_append ? O_APPEND : O_TRUNC;
					
					int fd = open(parser->current_token->value, flags, 0644);
					if (fd >= 0)
						close(fd);
					
					advance_token(parser);
				}
				continue;
			}
			else if (parser->current_token->type == TOKEN_REDIR_IN)
			{
				int fd = open(parser->current_token->value, O_RDONLY);
				if (fd < 0)
					sflag = 1;
			}
			advance_token(parser);
		}
		
		// Reset parser position
		parser->current_token = start_token;
	}
	
	return (cmd);
}

/* Parse a pipeline */
static t_cmd_node *parse_pipeline(t_parser *parser)
{
	t_cmd_node *first_cmd;
	t_cmd_node *current_cmd;
	t_cmd_node *next_cmd;
	int pipe_count = 0;

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
		
		// Check for syntax error: empty pipe (e.g., cmd | | cmd)
		if (parser->current_token && parser->current_token->type == TOKEN_PIPE)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
			parser->error = 1;
			return (first_cmd);
		}
		
		// Set the current command type to PIPE
		current_cmd->type = PIPE;
		
		// Parse the next command in the pipeline
		next_cmd = parse_simple_command(parser);
		if (!next_cmd)
		{
			// Handle case where there's nothing after the pipe
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
			parser->error = 1;
			return (first_cmd);
		}
		
		// Link the commands
		current_cmd->next = next_cmd;
		current_cmd = next_cmd;
	}
	
	// Debug output to see how many pipes were found
	// ft_putstr_fd("minishell debug: Pipeline with ", 2);
	// char *pipe_str = ft_itoa(pipe_count, &parser->exec->gc);
	// if (pipe_str)
	// {
	// 	ft_putstr_fd(pipe_str, 2);
	// }
	// ft_putstr_fd(" pipes created\n", 2);
	
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
	
	// Check for errors
	if (parser->error)
	{
		ft_putstr_fd("minishell: syntax error\n", 2);
		exec->exit_status = 2;
		return (NULL);
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
		// Check for empty commands
		if (!current->arr || !current->arr[0])
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

/* Main parse function for external call */
t_cmd_node *parse_command_line(char *input, t_exec *exec)
{
	t_cmd_node *cmd_list;
	
	// Parse the input
	cmd_list = parse_input(input, exec);
	
	// Check for syntax errors
	if (check_syntax(cmd_list, exec))
		return (NULL);
	
	// Store the command list in the exec structure
	exec->cmd_list = cmd_list;
	
	return (cmd_list);
} 