/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runcmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 21:15:56 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/10 17:16:03 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"




#include "minishell.h"

// -----------------------------------------------------------------------------
// Modo bash: si la línea contiene SOLO redirecciones (p.ej. "> a"),
// NO se ejecuta ningún comando ni se hace fork. Solo se aplican los efectos
// colaterales: crear/truncar/validar archivos o consumir heredoc.
// Esta función recorre la cadena de REDIR (node->left) y hace open/close o heredoc.
// Devuelve 0 si todo OK, -1 si alguna redirección falla.
// -----------------------------------------------------------------------------
int	apply_redirs_only(t_tree *node)
{
    while (node && is_node_type(node, "REDIR"))
    {
		int hfd;
		
        t_redir *r = (t_redir *)node->obj;
        if (r->mode & MODE_HEREDOC)
        {
            hfd = handle_heredoc(r->file); // consume heredoc
            if (hfd < 0)
                return -1;
            close(hfd); // no se usa; solo validar/consumir
        }
        else
        {
            int fd = open(r->file, r->mode, 0644);
            if (fd < 0)
                return -1;
            close(fd); // efecto: crear/truncar/validar
        }
        node = node->left; // siguiente redirección (más interna)
    }
    return 0;
}


// Ejecuta un EXEC: builtins en padre si procede; si no, fork + redirs + execve
void run_exec(t_tree *tree, t_data *data)
{
    t_exec *exec;
    pid_t   pid;
	
	exec = (t_exec *)tree-> obj;
    // Caso especial: EXEC sin argv[0] (línea de SOLO redirecciones: "> a")
    if (!exec || !exec->argv[0])
    {
        if (apply_redirs_only(tree->left) < 0)
            perror("redirection");
        return ; // sin fork ni exec
    }

    // Builtins en el padre (los que alteran el estado del shell)
    if (is_builtin_parents(exec) && exec)
	{
		execute_builtin_parents(exec);
		return ;
	}
		
    // Resto: ejecutamos en hijo
    pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        return;
    }
    if (pid == 0)
    {
        // Hijo: aplica redirecciones reales (dup2) y ejecuta
        runcmd(tree->left, data);
        if (is_builtin_child(exec) && exec)
            execute_builtin_child(exec);
        else
            execute_external_command(exec);
    }
    else
    {
        waitpid(pid, NULL, 0);
    }
}
/*
 * Ejecuta un nodo de tipo EXEC.
 * Si el comando es un built-in, lo ejecuta directamente en el proceso actual.
 * Si no, crea un proceso hijo para ejecutar el comando externo usando execve.
 * Esta función maneja la ejecución de comandos simples en el minishell.
 */

/*
void	run_exec(t_tree *tree, t_data *data)
{
	t_exec	*exec;
	pid_t	pid;
	
	//si no es NULL, hacemos casting: caso parse > a
	if (tree ->obj)
		exec = (t_exec *)tree->obj;
	if (is_builtin_parents(exec))
		ft_cd(exec->argv);
	
	else
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork failed\n");
			return ;
		}
		//creo que el problema es que no entra  la proceso hijo
		if (pid == 0)
		{
			runcmd(tree->left, data);
			if (is_builtin_child(exec))
				execute_builtin_child(exec);
			else
				execute_external_command(exec);
			//termina el proceso hijo: caso parse > a
			exit(1);
		}
		if (pid > 0)
			waitpid(pid, NULL, 0);
	}
}

*/

/*
 * Ejecuta un comando externo en el proceso hijo.
 * Busca la ruta del comando, la copia a un buffer y llama a execve.
 * Si ocurre un error, muestra un mensaje y termina el proceso.
 */
void	execute_external_command(t_exec *exec)
{
	char		*path;
	char		path_buf[PATH_MAX];
	//extern char	**environ;

	path = get_command_path(exec->argv[0]);
	if (!path)
		panic("command not found\n");
	ft_strlcpy(path_buf, path, sizeof(path_buf));
	free(path);
	execve(path_buf, exec->argv, env());
	panic("execve failed\n");
}

/*
 * Ejecuta un nodo de tipo REDIR, realizando la redirección de archivos
 * y luego ejecutando el comando hijo.
 */
void	run_redir(t_tree *tree, t_data *data)
{
	t_redir	*redir;
	int		fd;

	redir = (t_redir *)tree->obj;
	if (redir->mode & MODE_HEREDOC)
		fd = handle_heredoc(redir->file);
	else
		fd = open(redir->file, redir->mode, 0644);
	if (fd < 0)
		panic("open failed\n");
	if (dup2(fd, redir->fd) < 0)
		panic("dup2 failed\n");
	close(fd);
	runcmd(tree->left, data);
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
	waitpid(pid_left, NULL, 0);
	waitpid(pid_right, NULL, 0);
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