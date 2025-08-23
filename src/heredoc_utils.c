/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 15:04:57 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/22 15:08:55 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_heredoc_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
		panic("pipe failed\n", EXIT_FAILURE);
}

int	is_delimiter(const char *line, const char *cmp_delim, size_t len)
{
	return (ft_strncmp(line, cmp_delim, len + 1) == 0);
}

void	heredoc_loop(int write_fd, const char *cmp_delim, int quoted_delim)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (is_delimiter(line, cmp_delim, ft_strlen(cmp_delim)))
		{
			free(line);
			break ;
		}
		process_heredoc_line(write_fd, line, quoted_delim);
		free(line);
	}
}
