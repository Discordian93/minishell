/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esteizag <esteizag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 12:53:07 by esteizag          #+#    #+#             */
/*   Updated: 2023/11/15 12:53:12 by esteizag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t		len;
	char		*new_s;
	size_t		i;

	len = ft_strlen(s);
	new_s = malloc(sizeof(char) * (len + 1));
	if (new_s == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (i < len)
	{
		new_s[i] = f(i, s[i]);
		i++;
	}
	new_s[i] = '\0';
	return (new_s);
}
/* #include <stdio.h>

char next_char(unsigned int i, char c) {
	printf("%i\n", i);
    return c + 1;
}



int main() {
    char s[] = "Hello, World!";
    char *result = ft_strmapi(s, next_char);
    printf("%s\n", result);  // Should print "Ifmmp-!Xpsme\""
    free(result);

    char s2[] = "abc";
    result = ft_strmapi(s2, next_char);
    printf("%s\n", result);  // Should print "bcd"
    free(result);

    return 0;
} */
