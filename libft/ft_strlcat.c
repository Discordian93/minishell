/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 12:40:05 by ypacileo          #+#    #+#             */
/*   Updated: 2024/06/30 14:05:28 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include <stdio.h>
#include<string.h>*/
#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	dest_len;
	size_t	src_len;
	size_t	i;

	i = 0;
	dest_len = ft_strlen(dest);
	src_len = ft_strlen(src);
	if (size == 0)
		return (src_len);
	if (size <= dest_len)
		return (size + src_len);
	while ((dest_len + i) < (size - 1) && src[i] != '\0')
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';
	return (dest_len + src_len);
}
/*int main()
{
	char dest[10] = "Hello";
    const char *src = " World";
    size_t dstsize = 10;

    size_t result = ft_strlcat(dest, src, dstsize);

    printf("Final string: %s\n", dest);  // Expected: "Hello"
    printf("Total length: %zu\n", result);  // Expected: 6 

    return 0;
}*/