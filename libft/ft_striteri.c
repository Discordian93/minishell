/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:52:31 by ypacileo          #+#    #+#             */
/*   Updated: 2024/06/29 17:29:28 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdio.h> 
#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	int	i;

	if (s == NULL)
		return ;
	i = 0;
	while (s[i] != '\0')
	{
		f(i, &s[i]);
		i++;
	}
}

/*void    ft_to_upper(unsigned int, char *c)
{
	if (*c >= 'a' && *c <= 'z')
		*c -= 32;
}
int main()
{
	char	str[] = "hOla mundo";
	ft_striteri(str, ft_to_upper);
    printf("%s\n",str);
}*/