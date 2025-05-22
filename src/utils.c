/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:19:58 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/21 07:16:52 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//my malloc and my free
void	*ft_malloc(t_gc *var, size_t size)
{
	t_mem_node	*new_node;
	void		*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	new_node = malloc(sizeof(t_mem_node));
	if (!new_node)
	{
		free(ptr);
		return (NULL);
	}
	new_node->ptr = ptr;
	new_node->next = var->head;
	var->head = new_node;
	return (ptr);
}

void	ft_free_all(t_gc *gc)
{
	t_mem_node	*curr;
	t_mem_node	*tmp;

	if (!gc || !gc->head)
		return ;
	curr = gc->head;
	gc->head = NULL;
	while (curr)
	{
		tmp = curr;
		curr = curr->next;
		if (tmp->ptr)
			free(tmp->ptr);
		free(tmp);
	}
}

int	is_valid_var_name(const char *name)
{
	int	i;

	if (!name || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
