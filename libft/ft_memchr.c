/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:36:11 by ypacileo          #+#    #+#             */
/*   Updated: 2024/06/08 18:00:48 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include <stdio.h>
#include <string.h>*/
#include "libft.h"

void	*ft_memchr(const void *str, int c, size_t n)
{
	char unsigned	*s;
	char unsigned	car;
	size_t			i;

	i = 0;
	s = (unsigned char *)str;
	car = (unsigned char)c;
	while (i < n)
	{
		if (s[i] == car)
			return (&s[i]);
		i++;
	}
	return (0);
}
/*int main(int argc, char ** argv)
{
  char * result;
  char car = 'm';
  if ( argc != 2 )
    printf( "Usage: %s string\n", argv[0] );
  else
  {
if ((result = ft_memchr( argv[1], 'm', ft_strlen(argv[1])) ) != NULL)
printf( "el caracter %c se encuentra en la posicion %ld\n"
,car, result - argv[1]);
else
printf( "Caracter %c no encontrado\n", car );
  }
}*/
