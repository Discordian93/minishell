/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:19:08 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/22 14:20:47 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_argv(char **argv)
{
	int	i;

	i = 0;
	while (i < MAXARGS && argv && argv[i])
	{
		free(argv[i]);
		i++;
	}
}

void	free_exec(t_exec *exec)
{
	if (!exec)
		return ;
	free_argv(exec->argv);
	free(exec);
}

void	free_redir(t_redir *redir)
{
	if (!redir)
		return ;
	free(redir->file);
	free(redir);
}

void	free_tree(t_tree *node)
{
	if (!node)
		return ;
	free_tree(node->left);
	free_tree(node->right);
	if (is_node_type(node, "EXEC"))
		free_exec((t_exec *)node->obj);
	else if (is_node_type(node, "REDIR"))
		free_redir((t_redir *)node->obj);
	free(node);
}
