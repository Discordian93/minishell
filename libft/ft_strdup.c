/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 17:06:31 by ypacileo          #+#    #+#             */
/*   Updated: 2024/07/04 15:15:29 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>*/
#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	length;
	size_t	i;
	char	*str;

	length = ft_strlen(s);
	str = malloc(sizeof(char) * (length + 1));
	if (str == 0)
		return (0);
	i = 0;
	while (i < length)
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

/*
int main() 
{
    // Cadena original
    const char *original = "Hola, mundo!";
    // Duplicar la cadena usando strdup
    char *duplicada = ft_strdup(original);
    // Verificar si la duplicación fue exitosa
    if (duplicada == NULL) 
	{
        printf("La duplicación de la cadena ha fallado.\n");
        return 1;
    }

    // Imprimir la cadena original y la duplicada
    printf("Cadena original: %s\n", original);
	printf("primera direccion del bloque original: %p\n",original);
    printf("Cadena duplicada: %s\n", duplicada);
	printf("primera direccion del bloque copia: %p\n",duplicada);
    // Liberar la memoria asignada para la cadena duplicada
    free(duplicada);

    return 0;
}
*/