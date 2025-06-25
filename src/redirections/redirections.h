/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 03:12:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/02/06 03:12:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTIONS_H
# define REDIRECTIONS_H

# include "../../include/minishell.h"

/* Error handling functions */
int		validate_redirection_file(t_exec *exec, const char *filename, int mode);

/* Main redirection handling */
void	handle_redirection(t_cmd_node *cmd, t_gc *gc);
void	parse_redirections(t_cmd_node *cmd, char **args);

/* Argument processing */
void	process_and_update_args(t_cmd_node *cmd, char **args);

/* Redirection setup and restoration */
int		setup_output_redirection(t_cmd_node *cmd, int *original_fd);
int		restore_output_redirection(int original_fd);
int		setup_input_redirection(t_cmd_node *cmd, int *original_fd);
int		restore_input_redirection(int original_fd);
int		handle_output_redirection(t_cmd_node *cmd);

/* Local redirection handling */
void	restore_og_redirections(int original_in, int original_out);
int		setup_input_redirection_local(t_cmd_node *cmd, int *original_in);
int		setup_output_redirection_local(t_cmd_node *cmd, int *original_out);
int		setup_redirections(t_cmd_node *cmd, int *original_in,
			int *original_out);

/* Heredoc functionality */
int		handle_heredoc(t_cmd_node *cmd, t_exec *exec);
void	cleanup_heredoc_files(t_cmd_node *cmd);
void	preprocess_attached_redirs(t_cmd_node *cmd, char ***args);
void	process_input_redirection(t_cmd_node *cmd, char **args);
void	process_output_redirection(t_cmd_node *cmd, char **args);
char	**filter_command_args(t_cmd_node *cmd, char **args);

char	**process_attached_redirections(char **args, t_gc *gc, int *position);
char	**extract_redirection_files(
			char **args, int **is_append, int *count, t_gc *gc);
char	**handle_redir_case(char **args, t_gc *gc, int *pos,
			t_redir_norm *red);
void	display_file_error(const char *file, const char *message, t_gc *gc);
int		is_redirection(const char *str);
int		find_redir_pos(const char *arg, char *redir_type);
void	print_split_debug(char *cmd, char *redir, char *file);
void	create_file_for_redir(char *filename, char *arg, int pos);
int		create_and_read_heredoc(char *delimiter, t_exec *exec, t_cmd_node *cmd);
int		read_heredoc_content(const char *delimiter, t_exec *exec, int fd);
int		create_heredoc_temp_file(char *f_name);
#endif
