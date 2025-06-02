/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 02:51:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/02/06 02:51:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CD_H
# define CD_H

# include "minishell.h"

/* Core cd functionality */
int			ft_cd(t_exec *exec, t_cmd_node *cmd);
int			cd_change_directory(const char *path, t_exec *exec);
int			cd_validate_args(t_cmd_node *cmd, t_exec *exec);
char		*cd_get_current_pwd(t_exec *exec);
const char	*cd_resolve_path(t_exec *exec, t_cmd_node *cmd,
				char **expanded_path);

/* cd utility functions */
void		cd_update_pwd_vars(t_exec *exec, const char *old_pwd);
char		*cd_get_home_path(t_exec *exec);
char		*cd_expand_home_path(t_exec *exec, const char *arg);
char		*cd_get_oldpwd(t_exec *exec);

#endif
