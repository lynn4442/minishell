/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 03:15:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/02/06 03:15:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PWD_H
# define PWD_H

# include "../../include/minishell.h"

/* Forward declaration */
t_env_var	*get_env_var(t_exec *exec, const char *key);

/* PWD builtin functionality */
char		*get_current_directory(t_exec *exec);
void		ft_pwd(t_exec *exec);

#endif
