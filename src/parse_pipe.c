/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 12:04:21 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/22 12:43:24 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*make_pipe_node(t_tree *left, t_tree *right)
{
	t_tree	*pipe_node;

	pipe_node = create_tree_node(NULL, "PIPE");
	if (!pipe_node)
		return (NULL);
	pipe_node->left = left;
	pipe_node->right = right;
	return (pipe_node);
}

t_tree	*parsepipe_tree(char *input)
{
	char	*pipe_pos;

	if (!check_token(input))
		return (NULL);
	pipe_pos = my_ft_strchr(input, '|');
	if (!pipe_pos)
		return (parseexec_tree(input));
	*pipe_pos = '\0';
	return (make_pipe_node (parseexec_tree(input), \
		parsepipe_tree(pipe_pos + 1)));
}
