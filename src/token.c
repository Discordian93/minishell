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
 * Objetivo: Calcula el tamaño del siguiente token, respetando comillas internas.
 * Devuelve el tamaño del token.
 */
int	token_size(const char *str)
{
	int		size;
	int		i;

	size = 0;
	i = 0;
	while (str[i] && str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
	{
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
 * Objetivo: Divide una cadena en tokens respetando comillas y espacios.
 * - Inicializa el array de tokens.
 * - Omite espacios y maneja correctamente las comillas.
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

/**
 * ft_token - Divide una cadena de entrada en tokens separados, 
 * 	omitiendo espacios, y devuelve un array de cadenas con cada 
 *  token individual.
 * 	@str: Cadena de entrada a tokenizar.
 *
 * Esta función toma una cadena de caracteres, la separa en tokens utilizando los
 * espacios como delimitadores (ignorando los espacios consecutivos), 
 * y devuelve un array dinámico de cadenas, donde cada elemento es 
 * un token. El array termina con un puntero NULL. En caso de error de 
 * memoria, libera los recursos asignadosy retorna NULL.
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



/*
int main()
{
	char *input = "\"cat lol.c '|' cat > lol.c\" '|'     ";
	char **token = ft_token(input);
	int i = 0;
	while(token[i] != NULL)
	{
		printf("%s\n", token[i]);
		i ++;
	}
	
	return (0);
}*/