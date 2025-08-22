/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:37:58 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/22 13:49:17 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Objetivo: Calcula el tamaño de una subcadena entre comillas.
 * str: puntero a la cadena, *i: índice actual (se actualiza).
 * Devuelve el tamaño de la subcadena entre comillas, incluyendo las comillas.
 */
int	quoted_token_size(const char *str, int *i)
{
	int		size;
	char	quote;

	size = 0;
	quote = str[*i];
	(*i)++;
	size++;
	while (str[*i] && str[*i] != quote)
	{
		(*i)++;
		size++;
	}
	if (str[*i] == quote)
	{
		(*i)++;
		size++;
	}
	return (size);
}

/*
 * Objetivo: Calcula el tamaño del siguiente token, respetando comillas internas
 * y tratando operadores como delimitadores.
 * Devuelve el tamaño del token.
 */
int	token_size(const char *str)
{
	int		size;
	int		i;
	int		op_len;

	size = 0;
	i = 0;
	op_len = check_operator(str, 0);
	if (op_len > 0)
		return (op_len);
	while (str[i] && str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
	{
		if (check_operator(str, i) > 0)
			break ;
		if (str[i] == '"' || str[i] == '\'')
			size += quoted_token_size(str, &i);
		else
		{
			i++;
			size++;
		}
	}
	return (size);
}

/*
 * Count tokens including operators as separate tokens
 */
/*
int	count_tokens(const char *str)
{
	int	count;
	int	size;

	count = 0;
	while (*str)
	{
		while (*str && (*str == ' ' || *str == '\t' || *str == '\n'))
			str++;
		if (*str)
		{
			size = token_size(str);
			if (size > 0)
			{
				count++;
				str += size;
			}
			else
				break ;
		}
	}
	return (count);
}
*/

/*
 * Objetivo: Divide una cadena en tokens respetando comillas, 
 * espacios y operadores.
 * - Inicializa el array de tokens.
 * - Omite espacios y maneja correctamente las comillas.
 * - Trata operadores como tokens separados.
 * - Devuelve un array de strings (tokens) o NULL en caso de error.
 */
char	**init_token_array(const char *str, int *len)
{
	char	**s;

	*len = count_words(str);
	if (*len <= 0)
		return (NULL);
	s = malloc((*len + 1) * sizeof(char *));
	if (!s)
		return (NULL);
	return (s);
}

/*
 * Copia un token de longitud size desde src a dst.
 */
void	copy_token(char *dst, const char *src, int size)
{
	int	k;

	k = 0;
	while (k < size)
	{
		dst[k] = src[k];
		k++;
	}
	dst[k] = '\0';
}
