/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:41:12 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 09:59:11 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <ctype.h>
# include <unistd.h>
# include <limits.h>
# include <string.h>
# include <stdlib.h>
# include <sys/wait.h>
#include <sys/stat.h>
# include <stdbool.h>
# include "libft/libft.h"
# include <signal.h>

extern int g_signal_received;

typedef struct s_mem_node
{
	void				*ptr;
	struct s_mem_node	*next;
}	t_mem_node;

typedef struct s_garbage_collector
{
	t_mem_node	*head;
}	t_gc;

void	*ft_malloc(t_gc *gc, size_t size);
void	ft_free_all(t_gc *gc);

//t_elem: la kel token
//t_type: type tabaa l command
//t_env_var: lal env ta asema
//t_cmd_node: la kel command

//int g_signal_received = 0;

typedef struct s_elem {
	char			*token;
	int				space_after;
	struct s_elem	*next;
}	t_elem;

typedef enum e_type {
	PIPE,
	SMP_CMD
}	t_type;

//typedef enum e_type {
//	PIPE_T = 0,
//	SMP_CMD_T = 1
//}	t_type;

//typedef enum e_special_char {
//	PIPE = '|',
//	REDIR_IN = '<',
//	REDIR_OUT = '>',
//	DOUBLE_REDIR_IN,
//	DOUBLE_REDIR_OUT,
//}	t_special_char;

//int is_special_char(char c) {
//	return (c == PIPE || c == REDIR_IN || c == REDIR_OUT || c == DOUBLE_REDIR_IN || c == DOUBLE_REDIR_OUT || c == SPACE || c == TAB);
//}

//split aa ases li aam bekhdo bel env w asemo la 3 parts 1 the key
//2 the boolean eza le2a = aw lae
//3 the value li after the =
typedef struct s_env_var {
	char				*key;
	bool 				equal;
	char				*value;
	char				*all;
	struct s_env_var	*next;
	struct s_env_var	*prev;
} t_env_var;

typedef struct s_cmd_node {
	char				**arr;
	char				*in;
	char				*out;
	t_type				type;
	char				*heredoc;
	int					append;
	int					err;
	int					ex_heredoc;
	struct s_exec		*exec;
	struct s_cmd_node	*next;
}	t_cmd_node;

typedef struct s_exec{
	t_cmd_node	*cmd_list;
	t_env_var	*env_list;
	int			exit_status;
	t_gc		gc;
}	t_exec;

//init_data
t_env_var	*create_env_var(t_exec *exec, char *name, char *value);
t_cmd_node	*create_cmd_node(t_exec *exec, char **args);
void		init_exec(t_exec *exec);
void		add_env_var(t_exec *exec, char *name, char *value);

//echo
void		print_arg(char *arg, t_env_var *env, t_exec *exec);
int			handle_output_redirection(t_cmd_node *cmd);
void		ft_echo(t_cmd_node *cmd, t_env_var *env, t_exec *exec);

//cd
int			change_dir(const char *path, t_exec *exec);
void		update_pwd_vars(t_exec *exec, const char *old_pwd);
char		*get_home_path(t_exec *exec);
char		*expand_home_path(t_exec *exec, const char *arg);
char		*handle_oldpwd(t_exec *exec);
int			ft_cd(t_exec *exec, t_cmd_node *cmd);

//export
int			ft_isspace(char c);
int			is_valid_var_name(const char *name);
void		add_or_update_env_var(t_gc *gc, t_env_var **env_list, const char *name, const char *value);
void		handle_export(t_gc *gc, t_env_var **env_list, char *arg);
void		swap_env_vars(t_env_var *a, t_env_var *b);
void		sort_env_vars(t_env_var *head);
void		ft_export(t_env_var *env_list);
int			execute_export(t_cmd_node *node, t_exec *exec);

//unset
void		ft_unset(t_exec *exec, char **args);
t_env_var	*remove_env_var(t_exec *exec, const char *name);
void		unset_env_var(t_exec *exec, const char *name);

//pwd
char		*get_current_directory(t_exec *exec);
void		ft_pwd(t_exec *exec);

//env
void		init_env(t_exec *exec, char **envp);
void		ft_env(t_exec *exec, char **args);
char		**get_path_from_env(t_exec *exec);
char		*get_env_value(t_env_var *env_list, char *var_name);
t_env_var	*get_env_var(t_exec *exec, const char *key);
void		update_shlvl(t_exec *exec);
char		**convert_env_to_array(t_exec *exec, t_gc *gc);
int         get_shell_level(t_exec *exec);

//exit
int			is_numeric(const char *str);
void		ft_exit(char **args, int last_exit_status, t_exec *exec);

//main
void		parse_and_execute(t_exec *exec, t_cmd_node *cmd);
void		process_and_update_args(t_cmd_node *cmd, char **args);
void		parse_redirections(t_cmd_node *cmd, char **args);
int			main(int ac,char **av,char **envp);

//redirections
void		handle_redirection(t_cmd_node *cmd, t_gc *gc);
void		execute_command(t_cmd_node *cmd);
void        execute_command_generic(t_exec *exec, t_cmd_node *cmd);

//utils
void		*ft_malloc(t_gc *var, size_t size);
void		ft_free_all(t_gc *gc);

//builtins
int			handle_builtin_command(t_exec *exec, t_cmd_node *cmd);
int			is_builtin_command(const char *cmd);

//pipes related
int		has_pipe(t_cmd_node *cmd);
void	execute_with_pipes(t_exec *exec, t_cmd_node *cmd_list);
void	execute_pipe(t_exec *exec, char ***commands, int cmd_count);
char	***split_by_pipe(char *input, t_exec *exec);
char    *find_command_path(t_exec *exec, const char *cmd);
char    **split_preserve_quotes(const char *input, t_gc *gc);

// Add these function prototypes
int setup_output_redirection(t_cmd_node *cmd, int *original_fd);
int restore_output_redirection(int original_fd);
int setup_input_redirection(t_cmd_node *cmd, int *original_fd);
int restore_input_redirection(int original_fd);

// quotations handling
int check_quotes(const char *input);
void print_with_quote_handling(const char *arg, t_env_var *env, t_exec *exec);
char *process_quotes(const char *str, t_env_var *env, t_exec *exec);

// Add these prototypes
void setup_interactive_signals(void);
void setup_child_signals(void);
void setup_parent_signals(void);
void cleanup_and_exit(t_exec *exec, int exit_code);
void handle_eof_signal(t_exec *exec);
int get_signal_exit_status(int status);

// Parser functions
t_cmd_node *parse_command_line(char *input, t_exec *exec);
t_cmd_node *parse_input(char *input, t_exec *exec);
t_cmd_node *parse_piped_commands(char *input, t_exec *exec);
int check_syntax(t_cmd_node *cmd_list, t_exec *exec);

#endif

