/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 18:37:16 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/22 19:14:57 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_free_check(char **tok, int n)
{
	g_status = 2;
	free_split(&tok, n);
	return (0);
}

int	is_pipe_token(char *token)
{
	return (ft_strncmp(token, "|", 1) == 0);
}

int	is_redir_token(char *token)
{
	return (
		ft_strncmp(token, "<", 1) == 0 || \
		ft_strncmp(token, ">", 1) == 0 || \
		ft_strncmp(token, ">>", 2) == 0 || \
		ft_strncmp(token, "<<", 2) == 0
	);
}
