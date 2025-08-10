/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 09:36:17 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/10 14:50:36 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
// Libera la memoria ya asignada si ocurre un error en medio del proceso
void free_split(char ***s, int index)
{
    int i = 0;
    while (i < index)
    {
        free((*s)[i]);
        i++;
    }
    free(*s);
    *s = NULL;
}



void    free_tree(t_tree *node)
{
	int		i;
	t_exec	*exec;
	t_redir	*redir;
	
    if (!node)
        return;
    free_tree(node->left);
    free_tree(node->right);

    if (is_node_type(node, "EXEC"))
    {
        exec = (t_exec *)node->obj;
        if (exec)
        {
            i = 0;
            while (i < MAXARGS && exec->argv[i])
                free(exec->argv[i++]);
            free(exec);
        }
        
    }
    else if (is_node_type(node, "REDIR"))
    {
        redir = (t_redir *)node->obj;
        free(redir->file);
        free(redir);
    }
    free(node);
}
    
/*
void    dbg(const char *msg, t_tree *n) {
    if (!n) return;
    dprintf(2, "%s: %s @%p  L=%p R=%p\n", msg, n->label, (void*)n, n->left, n->right);
}

void free_tree(t_tree *node)
{
    if (!node) return;
    dbg("enter", node);
    free_tree(node->left);
    free_tree(node->right);

    if (is_node_type(node, "EXEC")) {
        t_exec *e = (t_exec*)node->obj;
        for (int i=0; i<MAXARGS && e && e->argv[i]; ++i) {
            dprintf(2, " free argv[%d]=%s\n", i, e->argv[i]);
            free(e->argv[i]);
        }
        free(e);
    } else if (is_node_type(node, "REDIR")) {
        t_redir *r = (t_redir*)node->obj;
        dprintf(2, " free redir file=%s\n", r ? r->file : "(null)");
        if (r) { free(r->file); free(r); }
    }
    dprintf(2, " free node %s\n", node->label);
    free(node);
}
*/


void ft_free(t_data *data)
{
    if (!data)
        return ;
    if (data->input)
        free(data->input);
    if (data->new_prompt)
        free(data->new_prompt);
    if (data->tree)
        free_tree(data->tree);
    data->input = NULL;
    data->new_prompt = NULL;
    data->tree = NULL;
}

void free_and_exit(t_data *data, int status)
{
    ft_free(data);
    rl_clear_history();
    if (data)
        free(data);
    exit(status);
}
