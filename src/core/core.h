/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 03:30:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/02/06 03:30:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_H
# define CORE_H

# include "../../include/minishell.h"

//main.c
int			main(int ac, char **av, char **envp);

//init_data.c
t_cmd_node	*create_cmd_node(t_exec *exec, char **args);
void		init_exec(t_exec *exec);
int			count_args(char **args);
void		init_cmd_fields(t_cmd_node *cmd, t_exec *exec);
int			copy_args(t_exec *exec, t_cmd_node *cmd, char **args, int count);

// check_builtins.c
int			handle_builtin_command(t_exec *exec, t_cmd_node *cmd);
int			is_builtin_command(const char *cmd);
int			handle_echo_cd_pwd(t_exec *exec, t_cmd_node *cmd);
int			handle_env_export_unset(t_exec *exec, t_cmd_node *cmd);
int			handle_exit(t_exec *exec, t_cmd_node *cmd);
void		process_and_update_args(t_cmd_node *cmd, char **args);
char		*copy_input_string(char *input, t_gc *gc);

//main_utils
t_exec		*initialize_shell(t_gc *gc, char **envp);
void		process_command_line(t_exec *exec, char *input);
int			handle_user_input(t_exec *exec, char **input);
int			is_empty_input(char *input);

#endif
