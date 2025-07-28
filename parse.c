/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:17:43 by ypacileo          #+#    #+#             */
/*   Updated: 2025/07/28 16:40:33 by ypacileo         ###   ########.fr       */
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
        *mode = O_RDONLY;
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
    (*redir)->file = ft_strdup(file);
    (*redir)->mode = mode;
    (*redir)->fd = fd;
}

// Parsea comandos simples con múltiples redirecciones encadenadas
t_tree  *parseexec_tree(char input)
{
    char    **token;
    t_exec  *exec;
    t_redir *redir;
    t_tree  *root_exec;
    t_tree  *current;
    t_tree  *redir_node;
    int i, j, mode, fd;

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
    if (!token)
    {
        perror("Error de token\n");
        return (NULL);
    }
        
    i = 0;
    j = 0;

    root_exec = create_tree_node((void *)exec, "EXEC");
    current = root_exec;
    
    while (token[i] != NULL)
    {
        if (is_redirection(token[i]))
        {
            i++;
            if (!token[i])
                panic("Falta archivo después de redirección");
            get_redir_info(token[i - 1], &mode, &fd);
            ft_redir(&redir, token[i], mode, fd);
            redir_node = create_tree_node((void *)redir, "REDIR");
            current->left = redir_node;
            current = redir_node;

        }
        else
			exec->argv[j++] = ft_strdup(token[i]);
        i++;
    }
    
    if (token)
        free_split(&token, i);
    
    return (root_exec);
}



// Parsea pipes y crea un nodo del árbol
t_tree  *parsepipe_tree(char *input)
{
    char    *pipe_pos;
    char    *left_part;
    char    *right_part;
    t_tree  *left_node;
    t_tree  *right_node;
    t_tree  *pipe_node;

    pipe_pos = ft_strchr(input, '|');
    if (pipe_pos == NULL)
		return (parseexec_tree(input));

    *pipe_pos = '\0';
    left_part = input;
    right_part = pipe_pos + 1;
    
    left_node = parseexec_tree(left_part);
    right_node = parsepipe_tree(right_part);
    
    
    pipe_node = create_tree_node(NULL, "PIPE");
    pipe_node->left = left_node;
    pipe_node->right = right_node;
    
    return (pipe_node);
}