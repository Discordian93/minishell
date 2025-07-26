/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 09:36:17 by yuliano           #+#    #+#             */
/*   Updated: 2025/07/26 10:55:24 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/**
 * Cuenta el número de elementos en un array de strings
 * @param str: array de strings terminado en NULL
 * @return: número de elementos
 */
int count_split(char **str)
{
    int i = 0;
    
    if (!str)
        return (0);
    
    while (str[i] != NULL)
        i++;
    
    return (i);
}


void free_tree(t_tree_node *node)
{
	int		i;
	char	*pipe;
	t_exec	*exec;
	t_redir	*redir;
	
    if (!node)
        return;

    // Primero liberamos subárbol izquierdo (REDIR, EXEC dentro de PIPE...)
    free_tree(node->left);

    // Luego liberamos subárbol derecho (solo usado en PIPE)
    free_tree(node->right);

    // Liberamos el contenido del nodo según su tipo
    if (is_node_type(node, "EXEC"))
    {
        exec = (t_exec *)node->objeto;
        i = 0;
		while (exec -> argv[i] != NULL)
			free(exec->argv[i++]);
        free(exec);
    }
    else if (is_node_type(node, "REDIR"))
    {
        redir = (t_redir *)node->objeto;
        free(redir->file);
        free(redir);
    }
    else if (is_node_type(node, "PIPE"))
    {
        pipe = (char *)node ->objeto;
		free(pipe);
    }

    // Finalmente liberamos el nodo en sí
    free(node);
}


void ft_free(t_data *data)
{
	if(data->input)
		free(data->input);
	if (data->new_prompt)
		free(data->new_prompt);
	if(data->tree)
		free_tree(data->tree);
}