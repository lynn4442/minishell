/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 03:10:00 by lyoussef          #+#    #+#             */
/*   Updated: 2025/02/06 03:10:00 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUOTES_H
# define QUOTES_H

# include "../../minishell.h"

/* Main quote processing functions */
int		check_quotes(const char *input);
char	*process_quotes(const char *str, t_env_var *env, t_exec *exec);
void	print_with_quote_handling(const char *arg, t_env_var *env,
			t_exec *exec);

/* Quote processing helpers */
int		handle_quote(const char *str, t_quote_check *st);
int		process_quoted_text(const char *str, char **result,
			t_env_var *env, t_exec *exec);

/* Variable and special character handling */
int		handle_exit_status(char *res, int len, t_exec *exec);
char	*extract_var_name(const char *str, int start, int end);
int		process_special_chars(const char *str, t_quote_check *st,
			t_env_var *env, t_exec *exec);

#endif
