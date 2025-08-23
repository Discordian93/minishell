/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:10:03 by ypacileo          #+#    #+#             */
/*   Updated: 2024/06/02 14:15:28 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include <stdio.h>
#include <ctype.h>*/

#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
	{
		c = c + 32;
	}
	return (c);
}
/*
int main() {
    char upper = 'A';
    char lower = 'a';
    char symbol = '#';

    printf("%c -> %c\n", upper, tolower(upper));  // A -> a
    printf("%c -> %c\n", lower, tolower(lower));  // a -> a
    printf("%c -> %c\n", symbol, tolower(symbol));  // # -> #

    return 0;
}*/
