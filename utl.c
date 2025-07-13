/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:21:24 by ypacileo          #+#    #+#             */
/*   Updated: 2025/07/13 12:26:40 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



// Libera recursivamente todo el árbol
void free_tree(t_tree_node *root)
{
    if (!root)
        return;
    
    // Liberar recursivamente los subárboles
    free_tree(root->left);
    free_tree(root->right);
    
    // Liberar el objeto según su etiqueta
    if (root->objeto)
    {
        if (is_node_type(root, "EXEC"))
            free(root->objeto); // Liberar estructura exec
        else if (is_node_type(root, "REDIR"))
            free(root->objeto); // Liberar estructura redir
        else if (is_node_type(root, "PIPE"))
            free(root->objeto); // Liberar estructura pipe
    }
    
    // Liberar el nodo actual
    free(root);
}