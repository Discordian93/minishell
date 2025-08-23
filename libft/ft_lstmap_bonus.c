/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esteizag <esteizag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 10:27:08 by esteizag          #+#    #+#             */
/*   Updated: 2023/11/15 10:27:11 by esteizag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_list	*create_new_node(void *(*f)(void *), void *content)
{
	t_list	*new_node;

	new_node = malloc(sizeof(t_list));
	if (new_node == NULL)
		return (NULL);
	new_node->content = f(content);
	new_node->next = NULL;
	return (new_node);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*head;
	t_list	*current_node;
	t_list	*new_node;

	if (lst == NULL)
		return (NULL);
	head = create_new_node(f, lst->content);
	if (head == NULL)
		return (NULL);
	current_node = head;
	lst = lst->next;
	while (lst != NULL)
	{
		new_node = create_new_node(f, lst->content);
		if (new_node == NULL)
		{
			ft_lstclear(&head, del);
			return (NULL);
		}
		current_node->next = new_node;
		current_node = new_node;
		lst = lst->next;
	}
	return (head);
}
