/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:21:24 by ypacileo          #+#    #+#             */
/*   Updated: 2025/08/22 14:03:38 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	panic(char *msg, int status)
{
	perror(msg);
	exit(status);
}

t_tree	*create_tree_node(void *obj, char *label)
{
	t_tree	*new_node;

	new_node = malloc(sizeof(t_tree));
	if (!new_node)
		perror("malloc failed\n");
	new_node->label = label;
	new_node->obj = obj;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

int	is_node_type(t_tree *node, char *type)
{
	if (!node || !node->label || !type)
		return (0);
	return (ft_strncmp(node->label, type, ft_strlen(type)) == 0);
}

int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}
