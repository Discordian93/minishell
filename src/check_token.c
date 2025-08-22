/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 21:55:01 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/22 18:29:04 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	 * Objetivo: Verifica si el token actual de pipe es inválido.
	 * Retorna 1 si es inválido, 0 si es válido.
*/
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

/*
	 * Objetivo: Verifica si el token actual de redirección es inválido.
	 * Retorna 1 si es inválido, 0 si es válido.
*/
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

int	ft_free_check(char **tok, int n)
{
	status = 2;
	free_split(&tok, n);
	return (0);
}

/*
	 * Objetivo: Función principal para verificar la validez de los tokens.
	 * Retorna 1 si los tokens son válidos, 0 en caso contrario.
 */
int	check_token(char *input)
{
	char	**tok;
	int		i;

	tok = ft_token(input);
	if (!tok)
		return (0);
	i = 0;
	while (i < count_split(tok))
	{
		if (ft_strncmp(tok[i], "|", 1) == 0)
		{
			if (es_pipe_invalida(tok, i, count_split(tok)))
				return (ft_free_check(tok, count_split(tok)));
		}
		else if (ft_strncmp(tok[i], "<", 1) == 0 || ft_strncmp(tok[i],">", 1) == 0 || \
			ft_strncmp(tok[i], ">>", 2) == 0 || ft_strncmp(tok[i], "<<", 2) == 0)
		{
			if (es_redir_invalida(tok, i, count_split(tok)))
				return (ft_free_check(tok, count_split(tok)));
		}
		i++;
	}
	free_split(&tok, count_split(tok));
	return (1);
}
