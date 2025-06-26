/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 18:27:52 by lyoussef          #+#    #+#             */
/*   Updated: 2025/06/26 18:27:54 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quotes.h"

char	*process_quotes(const char *str, t_env_var *env, t_exec *exec)
{
	char	*result;
	int		max_len;

	if (!str)
		return (NULL);
	max_len = calculate_max_result_length(str, env, &exec->gc);
	if (max_len < 0)
		return (NULL);
	result = ft_malloc(&exec->gc, max_len);
	if (!result)
		return (NULL);
	process_quoted_text(str, &result, env, exec);
	return (result);
}

void	print_with_quote_handling(const char *str,
			t_env_var *env, t_exec *exec)
{
	char	*processed;

	if (!str)
		return ;
	processed = process_quotes(str, env, exec);
	if (processed)
		printf("%s", processed);
}
