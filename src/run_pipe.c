/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 22:21:39 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/19 23:00:15 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	sig_default();
	close(fd[READ_END]);
	if (dup2(fd[WRITE_END], STDOUT_FILENO) < 0)
		panic("dup2 write failed\n", EXIT_FAILURE);
	close(fd[WRITE_END]);
	runcmd(tree->left, data);
	free_and_exit(data, EXIT_SUCCESS);
}

void	run_pipe_child_right(t_tree *tree, t_data *data, int fd[2])
{
	sig_default();
	close(fd[WRITE_END]);
	if (dup2(fd[READ_END], STDIN_FILENO) < 0)
		panic("dup2 read failed\n", EXIT_FAILURE);
	close(fd[READ_END]);
	runcmd(tree->right, data);
	free_and_exit(data, EXIT_SUCCESS);
}

void	parent_process_pipe(pid_t pid_left, pid_t pid_right, int *st)
{
	sig_ignore();
	waitpid(pid_left, NULL, 0);
	waitpid(pid_right, st, 0);
	status = decode_wait_status(*st);
	sig_init();
}

/*
 * Ejecuta un nodo de tipo PIPE, creando un pipe y dos procesos hijos
 * para manejar la comunicación entre los comandos.
 */
void	run_pipe(t_tree *tree, t_data *data)
{
	int		fd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		st;

	if (pipe(fd) == -1)
		perror("pipe failed\n");
	pid_left = fork();
	if (pid_left == -1)
		perror("fork failed\n");
	if (pid_left == 0)
		run_pipe_child_left(tree, data, fd);
	pid_right = fork();
	if (pid_right == -1)
		perror("fork failed\n");
	if (pid_right == 0)
		run_pipe_child_right(tree, data, fd);
	close(fd[0]);
	close(fd[1]);
	parent_process_pipe(pid_left, pid_right, &st);
}
