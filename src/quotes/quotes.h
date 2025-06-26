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

# include "../../include/minishell.h"

/*                        MAIN QUOTE PROCESSING                              */

int		check_quotes(const char *input);
char	*process_quotes(const char *str, t_env_var *env, t_exec *exec);
char	*process_heredoc_quotes(const char *str, t_env_var *env, t_exec *exec);
void	print_with_quote_handling(const char *arg, t_env_var *env,
			t_exec *exec);

/*                        QUOTE PROCESSING HELPERS                           */

int		handle_quote(const char *str, t_quote_check *st);
int		process_quoted_text(const char *str, char **result,
			t_env_var *env, t_exec *exec);
int		calculate_max_result_length(const char *str, t_env_var *env, t_gc *gc);

/*                    VARIABLE AND SPECIAL CHARACTER HANDLING                */

int		handle_exit_status(char *res, int len, t_exec *exec);
char	*extract_var_name(const char *str, int start, int end, t_gc *gc);
int		process_special_chars(const char *str, t_quote_check *st,
			t_env_var *env, t_exec *exec);

#endif
