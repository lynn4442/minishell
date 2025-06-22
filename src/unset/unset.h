/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/01/19 16:00:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UNSET_H
# define UNSET_H

# include "../include/types.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

// unset_core.c
void		ft_unset(t_exec *exec, char **args);
void		unset_env_var(t_exec *exec, const char *name);

// unset_errors.c
void		print_unset_error(const char *var_name, t_exec *exec);
void		process_single_var(t_exec *exec, const char *var_name,
				int *had_error);
void		process_all_vars(t_exec *exec, char **args);

// unset_operations.c
t_env_var	*handle_first_node_removal(t_exec *exec, t_env_var *current);
t_env_var	*handle_middle_node_removal(t_env_var *current, t_env_var *prev);
t_env_var	*find_and_remove_node(t_exec *exec, const char *name);
t_env_var	*remove_env_var(t_exec *exec, const char *name);

#endif
