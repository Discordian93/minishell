/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:34:55 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/17 18:41:14 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Objetivo: Saltar todos los espacios, tabs y saltos de línea en la cadena.
 * Devuelve un puntero a la primera posición no espacio.
 */
const char	*skip_spaces(const char *s)
{
	while (*s == ' ' || *s == '\t' || *s == '\n')
		s++;
	return (s);
}




/*
 * Objetivo: Avanza el índice hasta encontrar la 
 * comilla de cierre o el final de la cadena.
 * Devuelve el nuevo índice después de la comilla 
 * de cierre o el final.
 */
int	skip_quoted(const char *s, int i)
{
	char	quote;

	quote = s[i];
	i++;
	while (s[i] && s[i] != quote)
		i++;
	if (s[i] == quote)
		i++;
	return (i);
}

/*
 * Objetivo: Avanza el índice hasta el final de la palabra,
 * considerando comillas como agrupadores. Si encuentra una comilla
 * sin cerrar, devuelve -1 para indicar error; de lo contrario,
 * devuelve el nuevo índice.
 */
int	skip_word(const char *s, int i)
{
	char	quote;

	while (s[i] != ' ' && s[i] != '\t' && s[i] != '\n' && s[i] != '\0')
	{
		if (s[i] == '"' || s[i] == '\'')
		{
			quote = s[i++];
			while (s[i] != quote && s[i] != '\0')
				i++;
			if (s[i] == '\0')
				return (-1);
		}
		if (s[i] != '\0')
			i++;
	}
	return (i);
}

/*
 * Objetivo: Cuenta cuántas palabras hay en la cadena,
 * considerando comillas como agrupadores. Si hay una comilla
 * sin cerrar, devuelve -1 para evitar errores posteriores.
 */
int	count_words(const char *s)
{
	int	words;
	int	i;
	int	next_i;

	words = 0;
	i = 0;
	while (s[i] != '\0')
	{
		while (s[i] == ' ' || s[i] == '\t' || s[i] == '\n')
			i++;
		if (s[i] != '\0')
		{
			words++;
			next_i = skip_word(s, i);
			if (next_i == -1)
				return (-1);
			i = next_i;
		}
	}
	return (words);
}
