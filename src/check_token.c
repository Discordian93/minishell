/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 21:55:01 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/22 18:38:33 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	es_pipe_invalida(char **tok, int i, int n)
{
	int	len;

	len = 0;
	if (tok[i])
		len = ft_strlen(tok[i]);
	if (len != 1)
		return (1);
	if (i == 0 || i == n - 1 || ft_strncmp(tok[i + 1], "|", 1) == 0)
		return (1);
	return (0);
}

int	es_redir_invalida(char **tok, int i, int n)
{
	int	len;

	len = 0;
	if (tok[i])
		len = ft_strlen(tok[i]);
	if (len > 2)
		return (1);
	if (i == n - 1)
		return (1);
	if (ft_strncmp(tok[i + 1], ">", 1) == 0 \
		|| ft_strncmp(tok[i + 1], "<", 1) == 0)
		return (1);
	return (0);
}

int	check_token(char *input)
{
	char	**tok;
	int		i;
	int		n;

	tok = ft_token(input);
	if (!tok)
		return (0);
	n = count_split(tok);
	i = 0;
	while (i < n)
	{
		if (is_pipe_token(tok[i]))
		{
			if (es_pipe_invalida(tok, i, n))
				return (ft_free_check(tok, n));
		}
		else if (is_redir_token(tok[i]))
		{
			if (es_redir_invalida(tok, i, n))
				return (ft_free_check(tok, n));
		}
		i++;
	}
	free_split(&tok, n);
	return (1);
}
