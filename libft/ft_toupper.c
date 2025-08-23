/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 13:40:37 by ypacileo          #+#    #+#             */
/*   Updated: 2024/06/02 14:04:14 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include <stdio.h>
#include <ctype.h>*/
#include "libft.h"

int	ft_toupper( int c)
{
	if (c >= 'a' && c <= 'z')
	{
		c = c - 32;
	}
	return (c);
}
/*
int main() {
    char lower = 'a';
    char upper = 'A';
    char symbol = '#';

    printf("%c -> %c\n", lower, ft_toupper(lower));  // a -> A
    printf("%c -> %c\n", upper, ft_toupper(upper));  // A -> A
    printf("%c -> %c\n", symbol, ft_toupper(symbol));  // # -> #

    return 0;
}*/
