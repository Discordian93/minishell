/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:58:45 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/22 12:56:20 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection(char *token)
{
	return (ft_strncmp(token, ">", 2) == 0
		|| ft_strncmp(token, "<", 2) == 0
		|| ft_strncmp(token, ">>", 3) == 0
		|| ft_strncmp(token, "<<", 3) == 0);
}

void	get_redir_info(char *token, int *mode, int *fd)
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
		*mode = MODE_HEREDOC;
		*fd = 0;
	}
	else if (ft_strncmp(token, "<", 2) == 0)
	{
		*mode = O_RDONLY;
		*fd = 0;
	}
}

void	ft_redir(t_redir **redir, char *file, int mode, int fd)
{
	*redir = malloc(sizeof(t_redir));
	if (!*redir)
		panic("malloc failed\n", EXIT_FAILURE);
	(*redir)->file = ft_strdup(file);
	(*redir)->mode = mode;
	(*redir)->fd = fd;
}

void	handle_redirection(t_tree **current, char **token, int *i)
{
	t_redir	*redir;
	t_tree	*redir_node;
	int		mode;
	int		fd;

	get_redir_info(token[*i], &mode, &fd);
	(*i)++;
	if (mode & MODE_HEREDOC)
		process_heredoc_redir(&redir, token[*i], fd);
	else
		process_normal_redir(&redir, token[*i], mode, fd);
	redir_node = create_tree_node((void *)redir, "REDIR");
	(*current)->left = redir_node;
	*current = redir_node;
}
