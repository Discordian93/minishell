/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 18:42:27 by ypacileo          #+#    #+#             */
/*   Updated: 2024/06/02 11:58:08 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include <stdio.h>
#include <string.h>*/

#include "libft.h"

size_t	ft_strlen(const char *str)
{
	size_t	counter;

	counter = 0;
	while (str[counter] != '\0')
		counter++;
	return (counter);
}
/*int main()
{
	//              5    10
	char *s = "hola, mundo"; //long 11
	//        01234  6789
	
	printf("Función original: %lu\n", strlen(s));
	printf("Función propia:   %lu\n", ft_strlen(s));
	// strlen devuelve la longitug de caracteres, excluyendo el caracter nulo
	return (0);
}*/
