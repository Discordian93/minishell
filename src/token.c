/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 22:47:01 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/22 13:39:20 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Check if current position is at an operator (>, <, >>, <<, |)
 * Returns the length of the operator (1 or 2), or 0 if not an operator
 */
/*
int	is_operator_at(const char *str, int pos)
{
	if (!str[pos])
		return (0);
	
	// Check for two-character operators first
	if (str[pos] == '>' && str[pos + 1] == '>')
		return (2);
	if (str[pos] == '<' && str[pos + 1] == '<')
		return (2);
	
	// Check for single-character operators
	if (str[pos] == '>' || str[pos] == '<' || str[pos] == '|')
		return (1);
	
	return (0);
}
*/

/**
 * ft_token - Divide una cadena de entrada en tokens separados, 
 * 	omitiendo espacios, y devuelve un array de cadenas con cada 
 *  token individual. Trata operadores como tokens separados.
 * 	@str: Cadena de entrada a tokenizar.
 *
 * Esta función toma una cadena de caracteres, 
 * la separa en tokens utilizando los
 * espacios y operadores como delimitadores 
 * (ignorando los espacios consecutivos), 
 * y devuelve un array dinámico de cadenas, donde cada elemento es 
 * un token. El array termina con un puntero NULL. En caso de error de 
 * memoria, libera los recursos asignados y retorna NULL.
 */
char	**ft_token(const char *str)
{
	int		i;
	int		size;
	int		len;
	char	**s;

	s = init_token_array(str, &len);
	if (!s)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str = skip_spaces(str);
		size = token_size(str);
		s[i] = malloc(size + 1);
		if (!s[i])
		{
			free_split(&s, i);
			return (NULL);
		}
		copy_token(s[i], str, size);
		str += size;
		i++;
	}
	s[i] = NULL;
	return (s);
}
