/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 03:16:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/02/06 03:16:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXIT_H
# define EXIT_H

# include "../../include/minishell.h"

void	ft_exit(char **args, int last_exit_status, t_exec *exec);
void	print_numeric_error(const char *arg);
void	print_too_many_args_error(void);
int		normalize_exit_code(int exit_code);
void	handle_no_arguments(t_exec *exec);

#endif
