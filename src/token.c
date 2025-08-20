/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 22:47:01 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/19 22:07:59 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Check if current position is at an operator (>, <, >>, <<, |)
 * Returns the length of the operator (1 or 2), or 0 if not an operator
 */
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
	
	// If we start with an operator, return its length
	op_len = is_operator_at(str, 0);
	if (op_len > 0)
		return (op_len);
	
	// Otherwise, collect characters until we hit a separator or operator
	while (str[i] && str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
	{
		// Stop if we hit an operator (unless inside quotes)
		if (is_operator_at(str, i) > 0)
			break;
			
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
int	count_tokens(const char *str)
{
	int	count;
	int	size;

	count = 0;
	while (*str)
	{
		// Skip spaces
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
				break;
		}
	}
	return (count);
}

/*
 * Objetivo: Divide una cadena en tokens respetando comillas, espacios y operadores.
 * - Inicializa el array de tokens.
 * - Omite espacios y maneja correctamente las comillas.
 * - Trata operadores como tokens separados.
 * - Devuelve un array de strings (tokens) o NULL en caso de error.
 */
char	**init_token_array(const char *str, int *len)
{
	char	**s;

	*len = count_tokens(str);
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

/**
 * ft_token - Divide una cadena de entrada en tokens separados, 
 * 	omitiendo espacios, y devuelve un array de cadenas con cada 
 *  token individual. Trata operadores como tokens separados.
 * 	@str: Cadena de entrada a tokenizar.
 *
 * Esta función toma una cadena de caracteres, la separa en tokens utilizando los
 * espacios y operadores como delimitadores (ignorando los espacios consecutivos), 
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