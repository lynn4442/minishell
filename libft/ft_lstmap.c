/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyoussef <lyoussef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 14:35:47 by lyoussef          #+#    #+#             */
/*   Updated: 2024/06/21 15:39:04 by lyoussef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*start;
	t_list	*current;

	if (!lst || !f || !del)
		return (NULL);
	start = ft_lstnew(f(lst->content));
	current = start;
	while (lst && lst->next)
	{
		if (!current)
			return (NULL);
		current->next = ft_lstnew(f(lst->next->content));
		current = current->next;
		lst = lst->next;
	}
	return (start);
}
