/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 11:57:27 by ypacileo          #+#    #+#             */
/*   Updated: 2024/06/09 13:17:02 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include <stdio.h>
#include <string.h>*/
#include "libft.h"

char	*ft_strnstr(const char *mainChain, const char *subChain, size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (*subChain == '\0')
		return ((char *)mainChain);
	while (i < n && mainChain[i] != '\0')
	{
		if (mainChain[i] == *subChain)
		{
			j = 0;
			while (mainChain[i + j] == subChain[j] && i + j < n)
			{
				j++;
				if (subChain[j] == '\0')
					return ((char *)&mainChain[i]);
			}
		}
		i++;
	}
	return (0);
}

/*int main() 
{
    const char *haystack = "Hola Mundo";
    const char *needle = "Mundo";
    size_t len = 8;

    char *result = ft_strnstr(haystack, needle, len);

    if (result) {
        printf("Encontrado: %s\n", result);
    } else {
        printf("No encontrado.\n");
    }

    return 0;
}*/