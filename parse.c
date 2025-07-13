/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:17:43 by ypacileo          #+#    #+#             */
/*   Updated: 2025/07/13 12:28:10 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Función auxiliar para verificar si un token es una redirección
int is_redirection(char *token)
{
    return (ft_strncmp(token, ">", 1) == 0 || 
            ft_strncmp(token, "<", 1) == 0 ||
            ft_strncmp(token, ">>", 2) == 0 ||
            ft_strncmp(token, "<<", 2) == 0);
}


// Función auxiliar para determinar el tipo de redirección
void get_redir_info(char *token, int *mode, int *fd)
{
    if (ft_strncmp(token, ">>", 2) == 0)
    {
        *mode = O_WRONLY | O_CREAT | O_APPEND;
        *fd = 1;
    }
    else if (ft_strncmp(token, ">", 1) == 0)
    {
        *mode = O_WRONLY | O_CREAT | O_TRUNC;
        *fd = 1;
    }
    else if (ft_strncmp(token, "<<", 2) == 0)
    {
        *mode = O_RDONLY; // Heredoc - necesita tratamiento especial
        *fd = 0;
    }
    else if (ft_strncmp(token, "<", 1) == 0)
    {
        *mode = O_RDONLY;
        *fd = 0;
    }
}



void    ft_redir(t_redir **redir,char *file, int mode, int fd)
{
    *redir = malloc(sizeof(t_redir));
    if (!redir)
        panic("malloc failed");
    (*redir)->file = file;
    (*redir)->mode = mode;
    (*redir)->fd = fd;
}

// Parsea comandos simples con múltiples redirecciones encadenadas
t_tree_node *parseexec_tree(char *input)
{
    char **token;
    t_exec *exec;
    t_redir *redir;
    t_tree_node *exec_node;
    t_tree_node *current_node;
    t_tree_node *redir_node;
    int i, j, mode, fd;

    // Crear estructura exec
    exec = malloc(sizeof(t_exec));
    if (!exec)
        panic("malloc failed");
    

    j = 0;
    while(j < MAXARGS)
    {
        exec->argv[j] = NULL;
        j++;
    }

    token = ft_token(input);
    i = 0;
    j = 0;
    
    // Crear nodo EXEC base
    exec_node = create_tree_node((void *)exec, "EXEC");
    current_node = exec_node;
    
    while (token[i] != NULL)
    {
        if (is_redirection(token[i]))
        {
            i++; // Avanzar al nombre del archivo
            if (!token[i])
                panic("Falta archivo después de redirección");
            
            // Obtener información de la redirección
            get_redir_info(token[i - 1], &mode, &fd);
            
            ft_redir(&redir, token[i], mode, fd);
            
            // Crear nodo de redirección
            redir_node = create_tree_node((void *)redir, "REDIR");
            
            // Encadenar: la redirección actual apunta al nodo anterior
            redir_node->left = current_node;
            
            // La redirección actual se convierte en el nodo actual
            current_node = redir_node;
        }
        else
        {
            // Es un argumento del comando
            exec->argv[j++] = token[i];
        }
        i++;
    }
    
    // Liberar tokens originales
    //free_tokens(token);
    
    return (current_node);
}

// Parsea pipes y crea un nodo del árbol
t_tree_node *parsepipe_tree(char *input)
{
    char *pipe_pos;
    char *left_part;
    char *right_part;
    t_tree_node *left_node;
    t_tree_node *right_node;
    t_tree_node *pipe_node;
    char *pipe;

    pipe_pos = ft_strchr(input, '|');
    if (pipe_pos == NULL)
		return (parseexec_tree(input));

    *pipe_pos = '\0';
    left_part = input;
    right_part = pipe_pos + 1;
    
    left_node = parseexec_tree(left_part);
    right_node = parsepipe_tree(right_part);
    
    pipe = malloc(sizeof(char));
    if (!pipe)
        panic("malloc failed");
    
    pipe_node = create_tree_node((void *)pipe, "PIPE");
    pipe_node->left = left_node;
    pipe_node->right = right_node;
    
    return (pipe_node);
}