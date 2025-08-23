/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 13:53:08 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/23 13:53:36 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*init_heredoc_child(int pipefd[2], char *delim, int *quoted_delim)
{
	char	*cmp_delim;

	*quoted_delim = is_wrapped_in_quotes(delim);
	cmp_delim = strip_wrapping_quotes(delim);
	if (!cmp_delim)
		panic("malloc failed\n", EXIT_FAILURE);
	close(pipefd[READ_END]);
	return (cmp_delim);
}

static void	run_heredoc_child(char *delim, int pipefd[2])
{
	char	*cmp_delim;
	int		quoted_delim;

	sig_default();
	cmp_delim = init_heredoc_child(pipefd, delim, &quoted_delim);
	heredoc_loop(pipefd[WRITE_END], cmp_delim, quoted_delim);
	free(cmp_delim);
	close(pipefd[WRITE_END]);
	exit(EXIT_SUCCESS);
}

static int	wait_heredoc_child(pid_t pid, int pipefd[2])
{
	int	st;

	close(pipefd[WRITE_END]);
	sig_ignore();
	waitpid(pid, &st, 0);
	sig_init();
	g_status = decode_wait_status(st);
	if (WIFSIGNALED(st) && WTERMSIG(st) == SIGINT)
	{
		close(pipefd[READ_END]);
		return (-1);
	}
	return (pipefd[READ_END]);
}

int	handle_heredoc(char *delim)
{
	int		pipefd[2];
	pid_t	pid;

	init_heredoc_pipe(pipefd);
	pid = fork();
	if (pid < 0)
	{
		panic("fork failed\n", EXIT_FAILURE);
		return (-1);
	}
	if (pid == 0)
		run_heredoc_child(delim, pipefd);
	return (wait_heredoc_child(pid, pipefd));
}
