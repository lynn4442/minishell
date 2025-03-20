/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:19:58 by lyoussef          #+#    #+#             */
/*   Updated: 2025/03/20 18:25:46 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

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

void	ft_free(t_gc *var, void *ptr)
{
	t_mem_node	*prev;
	t_mem_node	*curr;

	if (!var || !ptr)
		return ;
	curr = var->head;
	prev = NULL;
	while (curr)
	{
		if (curr->ptr == ptr)
		{
			if (prev)
				prev->next = curr->next;
			else
				var->head = curr->next;
			free(curr->ptr);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

