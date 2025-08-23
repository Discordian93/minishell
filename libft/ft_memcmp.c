/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 18:28:33 by ypacileo          #+#    #+#             */
/*   Updated: 2024/06/09 11:53:36 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include <stdio.h>
#include <string.h>*/

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;
	size_t			i;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	return (0);
}

/*int main() {
    // Ejemplo con memcmp
    char mem1[] = {0x01, 0x02, 0x03, 0x04};
    char mem2[] = {0x01, 0x02, 0x03, 0x05};
    int result_memcmp = ft_memcmp(mem1, mem2, 4);
    if (result_memcmp == 0) {
        printf("memcmp: Los bloques de memoria son iguales.\n");
    } else if (result_memcmp < 0) {
        printf("memcmp: mem1 es menor que mem2.\n");
    } else {
        printf("memcmp: mem1 es mayor que mem2.\n");
    }
    return 0;
}*/