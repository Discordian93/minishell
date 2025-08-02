/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:17:43 by ypacileo          #+#    #+#             */
/*   Updated: 2025/08/02 17:16:43 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Función auxiliar para verificar si un token es una redirección
int is_redirection(char *token)
{
    return (ft_strncmp(token, ">", 2) == 0 || 
            ft_strncmp(token, "<", 2) == 0 ||
            ft_strncmp(token, ">>", 3) == 0 ||
            ft_strncmp(token, "<<", 3) == 0);
}


// Función auxiliar para determinar el tipo de redirección
void get_redir_info(char *token, int *mode, int *fd)
{
    if (ft_strncmp(token, ">>", 3) == 0)
    {
        *mode = O_WRONLY | O_CREAT | O_APPEND;
        *fd = 1;
    }
    else if (ft_strncmp(token, ">", 2) == 0)
    {
        *mode = O_WRONLY | O_CREAT | O_TRUNC;
        *fd = 1;
    }
    else if (ft_strncmp(token, "<<", 3) == 0)
    {
        *mode = O_RDONLY;
        *fd = 0;
    }
    else if (ft_strncmp(token, "<", 2) == 0)
    {
        *mode = O_RDONLY;
        *fd = 0;
    }
}



void    ft_redir(t_redir **redir,char *file, int mode, int fd)
{
    *redir = malloc(sizeof(t_redir));
    if (!*redir)
        panic("malloc failed\n");
    (*redir)->file = ft_strdup(file);
    (*redir)->mode = mode;
    (*redir)->fd = fd;
}


// Inicializa una estructura t_exec para almacenar los argumentos del comando.
t_exec  *initialize_exec(void)
{
	t_exec  *exec;
	int     j;

	exec = malloc(sizeof(t_exec));
	if (!exec)
		panic("malloc failed\n");

	j = 0;
	while (j < MAXARGS)
	{
		exec->argv[j] = NULL;
		j++;
	}

	return (exec);
}

// Construye el árbol de ejecución para un comando simple con redirecciones.
t_tree  *build_exec_tree(t_exec *exec, char **token)
{
	t_tree  *root_exec;
	t_tree  *current;
	int     i;
	int		j;

	root_exec = create_tree_node((void *)exec, "EXEC");
	current = root_exec;

	i = 0;
	j = 0;
	while (token[i] != NULL)
	{
		if (is_redirection(token[i]))
			handle_redirection(&current, token, &i);
		else
			exec->argv[j++] = ft_strdup(token[i]);
		i++;
	}

	return (root_exec);
}

// Maneja las redirecciones encontradas en los tokens y actualiza el árbol de ejecución.
void handle_redirection(t_tree **current, char **token, int *i)
{
	t_redir *redir;
	t_tree  *redir_node;
	int     mode;
	int     fd;

	(*i)++;
	if (!token[*i])
		panic("missing file after redirection\n");
	get_redir_info(token[*i - 1], &mode, &fd);
	ft_redir(&redir, token[*i], mode, fd);
	redir_node = create_tree_node((void *)redir, "REDIR");
	(*current)->left = redir_node;
	*current = redir_node;
}

// Parsea comandos simples con múltiples redirecciones encadenadas
// y construye un árbol de ejecución para el comando.
t_tree  *parseexec_tree(char *input)
{
	char    **token;
	t_exec  *exec;
	t_tree  *root_exec;
	char **token_expan;

	exec = initialize_exec();
	if (!exec)
		return (NULL);

	token = ft_token(input);
	
	if (!token)
	{
		write(2,"token failed\n",14);
		return (NULL);
	}
	token_expan = expand_vars(token);
	root_exec = build_exec_tree(exec, token_expan);
	//free_split(&token, count_split(token));
	
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

