/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runcmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 21:15:56 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/19 23:09:13 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Ejecuta un comando externo en el proceso hijo.
 * Busca la ruta del comando, la copia a un buffer y llama a execve.
 * Si ocurre un error, muestra un mensaje y termina el proceso.
 */
void	execute_external_command(t_exec *exec)
{
	char		*path;
	char		path_buf[PATH_MAX];

	path = get_command_path(exec->argv[0]);
	if (!path)
		panic("command not found\n", 127);
	ft_strlcpy(path_buf, path, sizeof(path_buf));
	free(path);
	execve(path_buf, exec->argv, env());
	panic("execve failed\n", EXIT_FAILURE);
}

/*
 * Ejecuta el árbol de comandos recibido, 
 * manejando los diferentes tipos de nodos:
 * ejecución simple, redirección y pipes.
 */
void	runcmd(t_tree *tree, t_data *data)
{
	if (!tree)
		return ;
	if (is_node_type(tree, "EXEC"))
		run_exec(tree, data);
	else if (is_node_type(tree, "REDIR"))
		run_redir(tree, data);
	else if (is_node_type(tree, "PIPE"))
		run_pipe(tree, data);
}
