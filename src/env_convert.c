/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_convert.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 19:55:04 by lyoussef          #+#    #+#             */
/*   Updated: 2025/05/04 17:30:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_env_vars(t_env_var *env_list)
{
	int			count;
	t_env_var	*current;

	count = 0;
	current = env_list;
	while (current)
	{
		if (current->value)
			count++;
		current = current->next;
	}
	return (count);
}

char	*create_env_string(t_gc *gc, t_env_var *env_var)
{
	char	*temp;
	size_t	total_len;

	total_len = ft_strlen(env_var->key) + ft_strlen(env_var->value) + 2;
	temp = ft_malloc(gc, total_len);
	if (!temp)
		return (NULL);
	ft_strcpy(temp, env_var->key);
	ft_strlcat(temp, "=", total_len);
	ft_strlcat(temp, env_var->value, total_len);
	return (temp);
}

char	**convert_env_to_array(t_exec *exec, t_gc *gc)
{
	t_env_var	*current;
	char		**env_array;
	int			i;
	char		*temp;

	env_array = ft_malloc(gc, (count_env_vars(exec->env_list) + 1)
			* sizeof(char *));
	if (!env_array)
		return (NULL);
	i = 0;
	current = exec->env_list;
	while (current)
	{
		if (current->value)
		{
			temp = create_env_string(gc, current);
			if (temp)
				env_array[i++] = temp;
		}
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
