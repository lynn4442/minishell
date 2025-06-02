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

# include "../../minishell.h"

/* Main redirection handling */
void	handle_redirection(t_cmd_node *cmd, t_gc *gc);
void	parse_redirections(t_cmd_node *cmd, char **args);

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

#endif
