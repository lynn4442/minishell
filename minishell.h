/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:41:12 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/07 21:05:09 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdlib.h>
# include "libft/libft.h"

//t_elem: la kel token
//t_type: type tabaa l command
//t_env_var: lal env ta asema
//t_cmd_node: la kel command

typedef struct s_elem {
	char			*token;
	int				space_after;
	struct s_elem	*next;
}	t_elem;

typedef enum e_type {
	PIPE,
	SMP_CMD,
}	t_type;

typedef struct s_env_var {
	char	*name;
	char	*value;
	struct s_env_var	*next;
	struct s_env_var	*prev;
}	t_env_var;

typedef struct s_cmd_node {
	char				**arr;
	char				*in;
	char				*out;
	t_type				type;
	char				*heredoc;
	int					append;
	int					err;
	int					ex_heredoc;
	struct s_cmd_node	*next;
}	t_cmd_node;

typedef struct s_exec{
	t_cmd_node	*cmd_list;
	t_env_var	*env_list;
	int			exit_status;
}	t_exec;

//init_data
t_env_var	*create_env_var(char *name, char *value);
void		free_env_list(t_env_var *env);
t_cmd_node	*create_cmd_node(char **args);
void		free_cmd_list(t_cmd_node *cmd);
void		free_elem_list(t_elem *elem);
t_exec		*init_exec();

//echo
char		*get_env_value(t_env_var *env_list, char *var_name);
void		print_arg(char *arg, t_env_var *env);
void		ft_echo(t_cmd_node *cmd, t_env_var *env, t_exec *exec);

//cd
t_env_var	*get_env_var(t_exec *exec, const char *name);
void		update_env_var(t_exec *exec, const char *name, const char *value);
int			change_dir(const char *path, t_exec *exec);
static void	update_pwd_vars(t_exec *exec, const char *old_pwd);
int			ft_cd(t_exec *exec, const char *arg);

//export
void		swap_env_vars(t_env_var *a, t_env_var *b);
void		sort_env_vars(t_env_var *head);
void		export_no_options(t_env_var *env_list);

//unset
t_env_var	*remove_env_var(t_exec *exec, const char *name);
void		free_env_var(t_exec *exec, t_env_var *node);
void		unset_env_var(t_exec *exec, const char *name);

//pwd
char		*get_current_directory(void);
void		execute_pwd(t_exec *exec);

//main
int main();

#endif
