/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/01/19 16:00:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ECHO_H
# define ECHO_H

# include "../../include/minishell.h"

// echo_core.c
void	ft_echo(t_cmd_node *cmd, t_env_var *env, t_exec *exec);

// echo_redirection.c
int		handle_echo_redirection(t_cmd_node *cmd,
			int *original_stdout, t_exec *exec);
int		restore_echo_output(int original_stdout, t_exec *exec);

// echo_utils.c
void	print_single_arg(char *arg, t_env_var *env, t_exec *exec);
int		parse_echo_options(char **args, int *i);
void	print_echo_args(char **args, int i, t_env_var *env, t_exec *exec);

#endif
