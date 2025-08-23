/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 17:56:11 by ypacileo          #+#    #+#             */
/*   Updated: 2024/06/30 14:02:36 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include <stdio.h>
#include <string.h>*/
#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	len;

	len = ft_strlen(src);
	if (size < 1)
		return (len);
	i = 0;
	while (i < (size - 1) && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (len);
}

/*int main() {
    char src[] = "hola, mndo";
    char dest[6]; // Búfer de destino suficientemente grande

    size_t copied = ft_strlcpy(dest, src, sizeof(dest));

    printf("Destino: %s\n", dest);
    printf("Caracteres copiados (longitud de src): %zu\n", copied);

    return 0;
}*/