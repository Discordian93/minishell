/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:21:24 by ypacileo          #+#    #+#             */
/*   Updated: 2025/08/15 20:40:52 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// panic(): Imprime mensaje de error y sale
void panic(char *msg) 
{
	perror(msg);
	exit(1);
}


// Crea un nuevo nodo del árbol con el objeto y etiqueta dados
t_tree	*create_tree_node(void *obj, char *label)
{
	t_tree *new_node;

	new_node = malloc(sizeof(t_tree));
	if (!new_node)
		perror("malloc failed\n");
	
	new_node->label = label;
	new_node->obj = obj;
	new_node->left = NULL;
	new_node->right = NULL;
	
	return (new_node);
}

// Verifica si un nodo es de un tipo específico
int	is_node_type(t_tree *node, char *type)
{
	if (!node || !node->label || !type)
		return (0);
	
	return (ft_strncmp(node->label, type, ft_strlen(type)) == 0);
}


/*
 * Ejecuta un nodo de tipo PIPE, creando un pipe y dos procesos hijos
 * para manejar la comunicación entre los comandos conectados por el pipe.
 * El proceso hijo izquierdo ejecuta el comando de la izquierda y redirige
 * su salida estándar al pipe.
 * El proceso hijo derecho ejecuta el comando de la derecha y redirige
 * su entrada estándar al pipe.
 * El padre espera a que ambos hijos terminen.
 */
void	run_pipe_child_left(t_tree *tree, t_data *data, int fd[2])
{
	signal(SIGINT,  SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(fd[READ_END]);
	if (dup2(fd[WRITE_END], STDOUT_FILENO) < 0)
		panic("dup2 write failed\n");
	close(fd[WRITE_END]);
	runcmd(tree->left, data);
	free_and_exit(data, EXIT_SUCCESS);
}

void	run_pipe_child_right(t_tree *tree, t_data *data, int fd[2])
{
	signal(SIGINT,  SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(fd[WRITE_END]);
	if (dup2(fd[READ_END], STDIN_FILENO) < 0)
		panic("dup2 read failed\n");
	close(fd[READ_END]);
	runcmd(tree->right, data);
	free_and_exit(data, EXIT_SUCCESS);
}

