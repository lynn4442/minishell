/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 03:13:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/02/06 03:13:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPES_H
# define PIPES_H

# include "minishell.h"

typedef struct s_r_variables
{
	int		i;
	int		j;
	int		k;
	int		escaped;
	char	quote;
	int		count;
	int		is_echo_command;
}	t_r_variables;
/* Forward declarations for functions from other modules */
char	*find_command_path(t_exec *exec, const char *cmd);
void	execute_command_supreme(t_exec *exec, t_cmd_node *cmd);
void	setup_child_signals(void);
char	**convert_env_to_array(t_exec *exec, t_gc *gc);

/* Main pipe functionality */
int		has_pipe(t_cmd_node *cmd);
void	execute_with_pipes(t_exec *exec, t_cmd_node *cmd_list);
void	execute_pipe(t_exec *exec, char ***commands, int cmd_count);

/* Pipe parsing and processing */
char	***split_by_pipe(char *input, t_exec *exec);
char	**split_preserve_quotes(const char *input, t_gc *gc);

void	init_r_variables(t_r_variables *vars);
void	skip_leading_spaces(const char *input, int *i, char *quote);
char	*extract_redirection(const char *input, int *i, t_gc *gc);
void	extract_argument_bounds(const char *input, int *i,
			char *quote, int *escaped);
int		count_arguments(const char *input, t_gc *gc);
void	skip_space(const char *input, int *i);
int		handle_redirection_pipe(const char *input, int *i, int *count);
void	skip_argument_token(const char *input, int *i,
			int *escaped, char *quote);
void	handle_escape_and_quotes(char c, int *escaped, char *quote);
#endif
