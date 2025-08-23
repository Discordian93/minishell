/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strampi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:25:59 by ypacileo          #+#    #+#             */
/*   Updated: 2024/06/29 16:49:13 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdio.h> 
#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	char	*str;

	if (!s)
		return (NULL);
	str = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		str[i] = f(i, s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}

/*char ft_to_upper(unsigned int, char c)
{
	if (c >= 'a' && c <= 'z')
		c -= 32;
	return (c);
}
int main()
{
	char	str[] = "hOla mundo";
	char	*s_final;

	s_final = ft_strmapi(str, ft_to_upper);
	printf("%s",s_final);
} */