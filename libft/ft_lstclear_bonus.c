/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esteizag <esteizag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 09:26:37 by esteizag          #+#    #+#             */
/*   Updated: 2023/11/15 09:26:38 by esteizag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	lstclear_rec(t_list *current_node, void (*del)(void*))
{
	if (current_node == NULL)
	{
		return ;
	}
	lstclear_rec(current_node->next, del);
	del(current_node->content);
	free(current_node);
}

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*current_node;

	if (lst == NULL)
	{
		return ;
	}
	current_node = *lst;
	lstclear_rec(current_node, del);
	*lst = NULL;
}

/* void ft_lstclear(t_list **lst, void (*del)(void*))
{
    t_list *tmp;

    while (*lst)
    {
        tmp = (*lst)->next;
        del((*lst)->content);
        free(*lst);
        *lst = tmp;
    }
    *lst = NULL;
}
 */
