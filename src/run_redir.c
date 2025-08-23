/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 22:19:19 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/19 23:07:44 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Recorre una cadena de nodos REDIR y aplica cada redirección.
 * Si encuentra un error en la redirección, termina el proceso con error.
 */
void apply_redirs_chain(t_tree *node)
{
    t_redir *r;
    int     fd;

    while (node && is_node_type(node, "REDIR"))
    {
        r = (t_redir *)node->obj;
        if (r->mode & MODE_HEREDOC)
        {
            /* For here-docs, already created during parsing */
            /* Only check that the descriptor can be opened for reading */
            fd = open(r->file, O_RDONLY);
            if (fd < 0)
                panic("redirection", EXIT_FAILURE);
            close(fd);
        }
        else
        {
            fd = open(r->file, r->mode, 0644);
            if (fd < 0)
                panic("redirection", EXIT_FAILURE);
            close(fd);
        }
        node = node->left;
    }
}

/*
 * Aplica únicamente las redirecciones encontradas en una cadena de nodos REDIR.
 * Ejecuta un proceso hijo para realizar las redirecciones
 */
void	apply_redirs_only(t_tree *node)
{
	pid_t	pid;
	int		st;

	if (is_node_type(node, "REDIR"))
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork failed");
			return ;
		}
		if (pid == 0)
		{
			sig_default();
			apply_redirs_chain(node);
			exit(EXIT_SUCCESS);
		}
		else
		{
			sig_ignore();
			waitpid(pid, &st, 0);
			g_status = decode_wait_status(st);
			sig_init();
		}
	}
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
	{
		panic("Unexpected heredoc in run_redir\n", EXIT_FAILURE);
	}
	fd = open(redir->file, redir->mode, 0644);
	if (fd < 0)
		panic("open failed\n", EXIT_FAILURE);
	if (dup2(fd, redir->fd) < 0)
		panic("dup2 failed\n", EXIT_FAILURE);
	close(fd);
	runcmd(tree->left, data);
}
