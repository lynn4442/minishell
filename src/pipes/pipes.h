/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 03:13:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/26 02:16:36 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPES_H
# define PIPES_H

# include "../../include/minishell.h"

char		*find_command_path(t_exec *exec, const char *cmd);
void		execute_command_supreme(t_exec *exec, t_cmd_node *cmd);
void		setup_child_signals(void);
char		**convert_env_to_array(t_exec *exec, t_gc *gc);
t_env_var	*get_env_var(t_exec *exec, const char *key);

int			handle_heredoc(t_cmd_node *cmd, t_exec *exec);
t_cmd_node	*parse_piped_commands(char *input, t_exec *exec);

void		execute_with_pipes(t_exec *exec, t_cmd_node *cmd_list);

// pipe parsing

void		setup_pipe_input(t_cmd_node *cmd, int prev_pipe_fd);
int			is_debug_enabled(t_exec *exec);
void		redirect_to_pipe(int next_pipe_fd, t_gc *gc);
void		redirect_to_file(int file_fd, t_gc *gc);
void		setup_tee_pipe(int file_fd, int next_pipe_fd, t_gc *gc);
void		print_output_error(t_cmd_node *cmd);
void		close_all_pipes(int pipe_count, int pipes[][2]);
int			count_pipeline_commands(t_cmd_node *cmd);
void		handle_builtin_or_empty(t_cmd_node *cmd);
void		setup_child_pipes(t_cmd_node *current, int i,
				int pipe_count, int pipes[][2]);
int			create_all_pipes(int pipe_count, int pipes[][2]);
int			wait_for_all_commands(t_exec *exec, pid_t *pids, int cmd_count);
void		fork_and_exec_commands(t_exec *exec, t_cmd_node *cmd_list,
				t_r_variables *var, char **env_array);

//debugging
void		print_parent_debug(t_exec *exec, int i, int cmd_count);
void		print_child_debug(t_cmd_node *current);
void		debug_print_cmd(t_exec *exec, t_cmd_node *cmd, int index);
void		debug_pipeline_commands(t_exec *exec,
				t_cmd_node *cmd_list, int cmd_count);
void		print_wait_debug(t_exec *exec, int i, int status);
void		print_redirection_debug_info(t_cmd_node *cmd);
void		print_command_debug_info(t_cmd_node *cmd);
int			is_debug_enabled(t_exec *exec);
#endif
