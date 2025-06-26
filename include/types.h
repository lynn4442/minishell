/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/01/19 16:00:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include <sys/types.h>
# include <stdbool.h>

typedef struct s_mem_node
{
	void				*ptr;
	struct s_mem_node	*next;
}	t_mem_node;

typedef struct s_garbage_collector
{
	t_mem_node	*head;
}	t_gc;

typedef enum e_type
{
	PIPE,
	SMP_CMD
}	t_type;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_EOF
}	t_token_type;

//core structures
typedef struct s_exec	t_exec;

typedef struct s_env_var
{
	char				*key;
	bool				equal;
	char				*value;
	char				*all;
	struct s_env_var	*next;
	struct s_env_var	*prev;
}	t_env_var;

typedef struct s_cmd_node
{
	char				**arr;
	char				*in;
	char				*out;
	t_type				type;
	char				**heredoc_delimiter;
	int					append;
	int					err;
	t_exec				*exec;
	struct s_cmd_node	*next;
}	t_cmd_node;

typedef struct s_exec
{
	t_cmd_node	*cmd_list;
	t_env_var	*env_list;
	int			exit_status;
	t_gc		gc;
	int			heredoc_counter;
}	t_exec;

//parsing structs
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

// lexing structs
typedef struct s_lexer
{
	char			*input;
	int				position;
	t_gc			*gc;
	int				error;
}	t_lexer;

// parsing struct
typedef struct s_parser
{
	t_token			*current_token;
	t_token			*token_list;
	t_exec			*exec;
	int				error;
}	t_parser;

// quotes parser
typedef struct s_quote_check
{
	int		i;
	int		len;
	char	*res;
	char	quote_type;
	int		escaped;
}	t_quote_check;

typedef struct s_elem
{
	char			*token;
	int				space_after;
	struct s_elem	*next;
}	t_elem;

typedef struct s_redirect_norm
{
	int		redir_pos;
	char	redir_type;
}	t_redir_norm;

typedef struct s_r_variables
{
	int		i;
	int		j;
	int		k;
	int		escaped;
	char	quote;
	int		count;
	int		is_echo_command;
	int		cmd_count;
	int		pipe_count;
	int		pipes[1024][2];
	pid_t	pids[1024];
}	t_r_variables;

typedef struct t_get_word_token
{
	char			*word;
	int				i;
	int				j;
	int				in_quotes;
	char			quote_type;
	char			*temp;
	int				escaped;
}	t_get_word_token;

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
}	t_parse_simple_cmd;

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
}	t_parse_pipeline;

#endif
