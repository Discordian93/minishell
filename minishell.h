/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 22:33:01 by yuliano           #+#    #+#             */
/*   Updated: 2025/07/12 22:53:39 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_SHELL
#define MINI_SHELL

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
# include "libft/libft.h"

# define MAXARGS 15

// Estructura genérica de nodo del árbol
typedef struct s_tree_node
{
    char *etiqueta;                  // Etiqueta que identifica el tipo: "EXEC", "REDIR", "PIPE"
    void *objeto;                    // Puntero genérico al objeto (exec, redir, pipe, etc.)
    struct s_tree_node *left;       // Nodo hijo izquierdo
    struct s_tree_node *right;      // Nodo hijo derecho
} t_tree_node;

// Estructuras específicas SIN campo type (ya no heredan de t_cmd)
typedef struct s_exec
{
    char *argv[MAXARGS];
} t_exec;

typedef struct s_redir 
{
    char *file;                      // Nombre del archivo destino o fuente
    int mode;                        // Modo de apertura (O_RDONLY, O_WRONLY, etc.)
    int fd;                          // File descriptor a redirigir (0 = stdin, 1 = stdout)
} t_redir;



// Funciones para manejo del árbol
t_tree_node *init_tree(void);
t_tree_node *create_tree_node(void *objeto, char *etiqueta);
void free_tree(t_tree_node *root);
void print_tree(t_tree_node *root, int depth);
int is_node_type(t_tree_node *node, char *tipo);

char **ft_token(const char *str);
#endif