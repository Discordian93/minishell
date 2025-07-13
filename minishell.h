/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 22:33:01 by yuliano           #+#    #+#             */
/*   Updated: 2025/07/13 12:31:51 by ypacileo         ###   ########.fr       */
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

typedef enum e_redir_type
{
    FILE_TRUNC,
    FILE_APPEND,
    FILE_IN,
    HEREDOC
} t_redir_type;

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
    t_redir_type   type;
    char *file;                      // Nombre del archivo destino o fuente
    int mode;                        // Modo de apertura (O_RDONLY, O_WRONLY, etc.)
    int fd;                          // File descriptor a redirigir (0 = stdin, 1 = stdout)
} t_redir;



// Funciones para manejo del árbol
t_tree_node *init_tree(void);
t_tree_node *create_tree_node(void *objeto, char *etiqueta);
void free_tree(t_tree_node *root);
int is_node_type(t_tree_node *node, char *tipo);
int is_redirection(char *token);
void get_redir_info(char *token, int *mode, int *fd);
void    ft_redir(t_redir **redir,char *file, int mode, int fd);
t_tree_node *parseexec_tree(char *input);
char **ft_token(const char *str);
t_tree_node *parsepipe_tree(char *input);
#endif