/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:35:07 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/25 09:18:08 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

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
