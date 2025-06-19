/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhussein <hhussein@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:41:12 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/16 21:02:27 by hhussein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../../libft/libft.h"
# include "minishell.h"
# include <ctype.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

/* Token types for lexical analysis */
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_EOF
}					t_token_type;

/* Lexer token structure */
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

/* Lexer context */
typedef struct s_lexer
{
	char			*input;
	int				position;
	t_gc			*gc;
	int				error;
}					t_lexer;

/* Parser context */
typedef struct s_parser
{
	t_token			*current_token;
	t_token			*token_list;
	t_exec			*exec;
	int				error;
}					t_parser;

typedef struct t_get_word_token
{
	char			*word;
	int				i;
	int				j;
	int				in_quotes;
	char			quote_type;
	char			*temp;
	int				escaped;
}					t_get_word_token;

typedef struct t_parse_simple_cmd
{
	t_cmd_node		*cmd;
	char			**args;
	int				arg_count;
	int				i;
	char			*filename;
	char			*last_input;
	char			*last_output;
	int				sflag;
	int				last_output_is_append;
	char			**heredoc_delimiter;
	int				heredoc_count;
}					t_parse_simple_cmd;

typedef struct t_parse_pipeline
{
	t_cmd_node		*first_cmd;
	t_cmd_node		*current_cmd;
	t_cmd_node		*next_cmd;
	int				pipe_count;
	t_token			*start_token;
	int				sflag;
	int				fd;
	int				is_append;
}					t_parse_pipeline;

t_lexer				*init_lexer(char *input, t_gc *gc);
int					is_whitespace(char c);
void				skip_whitespace(t_lexer *lexer);

t_token				*create_token(t_token_type type, char *value, t_gc *gc);
void				add_token(t_token **head, t_token *new_token);
void				init_get_word_token_struct(t_get_word_token *norm,
						t_lexer *lexer);
int					get_word_token_quotes(t_lexer *lexer,
						t_get_word_token *norm);
int					get_word_token_escaped(t_lexer *lexer,
						t_get_word_token *norm);
int					get_word_token_testcase(t_lexer *lexer,
						t_get_word_token *norm);
t_token				*get_word_token(t_lexer *lexer, t_gc *gc);
t_token				*get_next_token_pipe(t_lexer *lexer, t_gc *gc,
						char *current);
t_token				*input_error(t_lexer *lexer, t_gc *gc);
t_token				*get_next_token_input_r(t_lexer *lexer, t_gc *gc,
						char *current);
t_token				*get_next_token_output(t_lexer *lexer, t_gc *gc,
						char *current);
t_token				*get_next_token(t_lexer *lexer, t_gc *gc);
char				*copy_input_string(char *input, t_gc *gc);
t_token				*tokenize(char *input, t_gc *gc);
t_parser			*init_parser(t_token *tokens, t_exec *exec);
void				advance_token(t_parser *parser);
void				init_parse_simple_cmd_struct(t_parse_simple_cmd *parse);
void				parse_simple_command_token_word(t_token *current,
						t_parse_simple_cmd *parse, t_parser *parser);

void				parse_simple_command_output(t_token *current,
						t_parse_simple_cmd *parse, t_parser *parser);
int					parse_simple_command_input(t_token *current,
						t_parse_simple_cmd *parse, t_parser *parser);
void				advance_parser_position(t_parser *parser);
void				parse_simple_command_analyze_token(t_token *current,
						t_parse_simple_cmd *parse, t_parser *parser);
void				parse_simple_command_loop_2(t_token *current,
						t_parse_simple_cmd *parse, t_parser *parser);
t_cmd_node			*parse_simple_command(t_parser *parser);
void				init_parse_pipeline(t_parse_pipeline *parse,
						t_parser *parser);
void				parse_pipeline_token_word(t_parse_pipeline *parse,
						t_parser *parser);
void				pipe_file_creation(t_parse_pipeline *parse,
						t_parser *parser);
t_cmd_node			*parse_pipeline(t_parser *parser);
t_cmd_node			*parse_input(char *input, t_exec *exec);
int					check_syntax(t_cmd_node *cmd_list, t_exec *exec);
t_cmd_node			*parse_piped_commands(char *input, t_exec *exec);
t_cmd_node			*parse_command_line(char *input, t_exec *exec);
int	parse_simple_command_heredoc(t_token *current, t_parse_simple_cmd *parse, t_parser *parser);
#endif
