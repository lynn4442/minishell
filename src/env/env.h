/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:00:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/01/19 16:00:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include "../libft/libft.h"
# include "../include/types.h"

//env_core.c
void		init_env(t_exec *exec, char **envp);
void		ft_env(t_exec *exec, char **args);

//env_utils.c
char		*get_env_value(t_env_var *env_list, char *var_name);
t_env_var	*get_env_var(t_exec *exec, const char *key);
char		**get_path_from_env(t_exec *exec);

//env_var_utils.c
t_env_var	*create_env_var(t_exec *exec, char *name, char *value);
void		add_env_var(t_exec *exec, char *name, char *value);
void		update_env_var(t_gc *gc, t_env_var *var, const char *value);
t_env_var	*create_new_env_var(t_gc *gc, const char *name, const char *value);
void		add_or_update_env_var(t_gc *gc, t_env_var **env_list,
				const char *name, const char *value);

//env_convert.c
int			count_env_vars(t_env_var *env_list);
char		*create_env_string(t_gc *gc, t_env_var *env_var);
char		**convert_env_to_array(t_exec *exec, t_gc *gc);

//shlvl.c
int			get_shell_level(t_exec *exec);
void		update_shlvl(t_exec *exec);
#endif 