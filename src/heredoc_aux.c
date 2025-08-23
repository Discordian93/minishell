/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_aux.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 17:45:46 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/22 17:46:39 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Converts an integer to string (for fd conversion) */
char	*fd_to_string(int fd)
{
	char	*prefix;
	char	*fd_str;
	char	*result;

	prefix = "/dev/fd/";
	fd_str = ft_itoa(fd);
	if (!fd_str)
		return (NULL);
	result = ft_strjoin(prefix, fd_str);
	free(fd_str);
	return (result);
}

/* Handles heredoc redirection specifically */
void process_heredoc_redir(t_redir **redir, char *delim, int fd)
{
	int		heredoc_fd;
	char	*fd_path;

	heredoc_fd = handle_heredoc(delim);
	if (heredoc_fd < 0)
	{
		fd_path = ft_strdup("/dev/null");
		if (!fd_path)
			panic("malloc failed\n", EXIT_FAILURE);
		ft_redir(redir, fd_path, O_RDONLY, fd);
		free(fd_path);
		return ;
	}
	fd_path = fd_to_string(heredoc_fd);
	if (!fd_path)
		panic("malloc failed\n", EXIT_FAILURE);
	ft_redir(redir, fd_path, O_RDONLY, fd);
	free(fd_path);
}

/* Handles normal (non-heredoc) redirection */
void	process_normal_redir(t_redir **redir, char *token, int mode, int fd)
{
	char	*expanded_token;

	expanded_token = expand_token(token);
	ft_redir(redir, expanded_token, mode, fd);
	free(expanded_token);
}
