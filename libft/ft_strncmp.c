/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 17:40:01 by ypacileo          #+#    #+#             */
/*   Updated: 2024/06/30 14:21:45 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include <stdio.h>
#include <string.h>*/

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t		i;

	i = 0;
	while (i < n)
	{
		if (s1[i] != s2[i] || s1[i] == '\0' || s2[i] == '\0')
			return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
		i++;
	}
	return (0);
}

/*
int main() 
{
    
char str1[] = "Hola Mundo";
char str2[] = "Hola Mundo!";
int result_strncmp = ft_strncmp(str1, str2, 10);
if (result_strncmp == 0) {
printf("strncmp: Las cadenas son iguales en los primeros 10 caracteres.\n");
} else if (result_strncmp < 0) 
{
printf("strncmp: str1 es menor que str2 en los primeros 10 caracteres.\n");
} else 
{
printf("strncmp: str1 es mayor que str2 en los primeros 10 caracteres.\n");
}

    return 0;
}
*/
