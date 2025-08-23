/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:51:22 by ypacileo          #+#    #+#             */
/*   Updated: 2024/06/01 16:44:05 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include <stdio.h>
#include <string.h> */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*destiny;
	unsigned const char	*source;
	size_t				i;

	destiny = dest;
	source = src;
	i = 0;
	if (destiny < source)
	{
		while (i < n)
		{
			destiny[i] = source[i];
			i++;
		}
	}
	else if (destiny > source)
	{
		while (n--)
			destiny[n] = source[n];
	}
	return (dest);
}
/*
int main() {
    char str[] = "1234567890";
    printf("Antes: %s\n", str);


    ft_memmove(str + 2, str, 10);

    printf("Después: %s\n", str);

    return 0;
	
}*/