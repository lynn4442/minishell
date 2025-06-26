/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:41:12 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/26 02:23:21 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../../include/minishell.h"

// lexer

t_lexer				*init_lexer(char *input, t_gc *gc);
void				skip_whitespace(t_lexer *lexer);

//tokenization

t_token				*create_token(t_token_type type, char *value, t_gc *gc);
void				add_token(t_token **head, t_token *new_token);
t_token				*get_word_token(t_lexer *lexer, t_gc *gc);
t_token				*get_next_token(t_lexer *lexer, t_gc *gc);
t_token				*tokenize(char *input, t_gc *gc);

//tokenize helper
void				init_get_word_token_struct(t_get_word_token *norm,
						t_lexer *lexer);
int					get_word_token_quotes(t_lexer *lexer,
						t_get_word_token *norm);
int					get_word_token_escaped(t_lexer *lexer,
						t_get_word_token *norm);
int					get_word_token_testcase(t_lexer *lexer,
						t_get_word_token *norm);
t_token				*get_next_token_pipe(t_lexer *lexer, t_gc *gc,
						char *current);
t_token				*input_error(t_lexer *lexer, t_gc *gc);
t_token				*get_next_token_input_r(t_lexer *lexer, t_gc *gc,
						char *current);
t_token				*get_next_token_output(t_lexer *lexer, t_gc *gc,
						char *current);

// parser
t_parser			*init_parser(t_token *tokens, t_exec *exec);
void				advance_token(t_parser *parser);
void				advance_parser_position(t_parser *parser);
t_cmd_node			*parse_simple_command(t_parser *parser);
t_cmd_node			*parse_pipeline(t_parser *parser);
t_cmd_node			*parse_input(char *input, t_exec *exec);
t_cmd_node			*parse_command_line(char *input, t_exec *exec);

// parser helper
void				init_parse_simple_cmd_struct(t_parse_simple_cmd *parse);
void				parse_simple_command_token_word(t_token *current,
						t_parse_simple_cmd *parse, t_parser *parser);
void				parse_simple_command_output(t_token *current,
						t_parse_simple_cmd *parse, t_parser *parser);
int					parse_simple_command_input(t_token *current,
						t_parse_simple_cmd *parse, t_parser *parser);
int					parse_simple_command_heredoc(t_token *current,
						t_parse_simple_cmd *parse, t_parser *parser);
void				parse_simple_command_analyze_token(t_token *current,
						t_parse_simple_cmd *parse, t_parser *parser);
void				parse_simple_command_loop_2(t_token *current,
						t_parse_simple_cmd *parse, t_parser *parser);
void				set_command_redirections(t_cmd_node *cmd,
						t_parse_simple_cmd *parse, t_parser *parser);

// pipe parser
void				init_parse_pipeline(t_parse_pipeline *parse,
						t_parser *parser);
void				parse_pipeline_token_word(t_parse_pipeline *parse,
						t_parser *parser);
void				pipe_file_creation(t_parse_pipeline *parse,
						t_parser *parser);
int					process_redirection_token(t_parse_pipeline *parse,
						t_parser *parser);

// validation on the syntax
int					check_syntax(t_cmd_node *cmd_list, t_exec *exec);
int					validate_syntax(char *input, t_token *tokens, t_exec *exec);
int					check_pipe_syntax_errors(t_token *tokens, t_exec *exec);
int					check_redirection_syntax_errors(t_token *tokens,
						t_exec *exec);
int					check_quote_syntax_errors(char *input, t_exec *exec);
int					is_invalid_pipe(t_token *current, t_exec *exec);
int					print_redirection_error(t_token_type type, t_exec *exec);
void				update_quote_state(char c, int *single_q,
						int *double_q, int *escaped);

// input processing

char				*copy_input_string(char *input, t_gc *gc);
char				*process_quoted_token(char *val, t_parser *parser);
int					check_quotes(const char *input);

#endif
