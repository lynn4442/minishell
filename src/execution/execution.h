/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 03:14:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/02/06 03:14:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../../include/minishell.h"

/*                           CORE EXECUTION                                  */

void	parse_and_execute(t_exec *exec, t_cmd_node *cmd);

/*                           COMMAND DISPATCH                                */

int		execute_builtin_command(t_exec *exec, t_cmd_node *cmd);
int		execute_external_command(t_exec *exec, t_cmd_node *cmd);
int		handle_pipe_execution(t_exec *exec);
int		handle_builtin_without_redirect(t_exec *exec, t_cmd_node *cmd);
int		execute_command_with_redirect(t_exec *exec, t_cmd_node *cmd,
			int original_in, int original_out);

/*                           COMMAND PREPARATION                             */

void	report_cmd_failure(t_exec *exec, const char *cmd, int error_type);
int		handle_lost_command(t_exec *exec, const char *expanded_cmd);
int		prep_cmd_for_launch(t_exec *exec, t_cmd_node *cmd,
			char **expanded_cmd, char **cmd_path);

/*                           CHILD PROCESS HANDLING                          */

void	execute_child_process(t_cmd_node *cmd, char *cmd_path,
			char **env_array);
void	handle_child_exit_status(t_exec *exec, pid_t pid, t_cmd_node *cmd);
int		prepare_process_environment(t_exec *exec, char ***env_array);
int		create_and_execute_process(t_exec *exec, t_cmd_node *cmd,
			char *cmd_path, char **env_array);

/*                           PATH HANDLING                                   */

char	*is_path_absolute(t_exec *exec, const char *expanded_cmd);
char	*try_local_path(t_exec *exec, const char *expanded_cmd);
char	*cmd_search_prequel(t_exec *exec, const char *cmd);
char	*hunt_in_path_dirs(t_exec *exec, const char *expanded_cmd,
			char **path_dirs);

/*                           MISSING FUNCTION DECLARATIONS                   */

void	setup_child_signals(void);
char	**convert_env_to_array(t_exec *exec, t_gc *gc);
void	cleanup_heredoc_files(t_cmd_node *cmd);
void	ft_free_all(t_gc *gc);
int		handle_builtin_command(t_exec *exec, t_cmd_node *cmd);
void	execute_with_pipes(t_exec *exec, t_cmd_node *cmd_list);
int		setup_redirections(t_cmd_node *cmd, int *original_in, int *original_out);
void	restore_og_redirections(int original_in, int original_out);

#endif
