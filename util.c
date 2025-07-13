/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:21:24 by ypacileo          #+#    #+#             */
/*   Updated: 2025/07/13 18:48:40 by ypacileo         ###   ########.fr       */
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

/**
 * Concatena dos cadenas en una nueva cadena
 * @param s1: primera cadena (se libera automáticamente)
 * @param s2: segunda cadena a concatenar
 * @return: nueva cadena concatenada o NULL en caso de error
 */
char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*result;
	size_t	i;
	size_t	j;

	if (!s2)
		return (NULL);
	if (!s1)
	{
		s1 = malloc(1);
		if (!s1)
			return (NULL);
		s1[0] = '\0';
	}
	result = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!result)
	{
		free(s1);
		return (NULL);
	}
	i = 0;
	while (s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		result[i++] = s2[j++];
	result[i] = '\0';
	free(s1);
	return (result);
}