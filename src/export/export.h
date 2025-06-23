/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 03:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/02/06 03:00:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPORT_H
# define EXPORT_H

# include "minishell.h"

/* Core export functionality */
int		execute_export(t_cmd_node *node, t_exec *exec);
void	handle_export(t_gc *gc, t_env_var **env_list, char *arg);

/* Export command processing */
int		display_env_vars(t_exec *exec);
int		process_export_arg(t_exec *exec, char *arg);
int		process_export_args(t_cmd_node *node, t_exec *exec);

/* Export display and sorting */
void	swap_env_vars(t_env_var *a, t_env_var *b);
void	sort_env_vars(t_env_var *head);
void	print_env_var(t_env_var *var);
int		check_env_list(t_env_var *env_list);
void	ft_export(t_env_var *env_list);

/* Export validation functions */
int		validate_export_argument(const char *arg, t_exec *exec);
int		validate_unset_argument(const char *name, t_exec *exec);
int		is_valid_variable_name(const char *name);
int		is_valid_identifier_char(char c, int is_first);

/* Utility functions */
int		ft_isspace(char c);
void	add_or_update_env_var(t_gc *gc, t_env_var **env_list,
			const char *name, const char *value);

#endif
