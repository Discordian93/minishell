/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 13:34:07 by ypacileo          #+#    #+#             */
/*   Updated: 2024/07/13 13:54:46 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdio.h>
#include "libft.h"

int	ft_atoi(const char *s)
{
	size_t	i;
	long	number;
	int		sign;

	i = 0;
	sign = 1;
	number = 0;
	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\n'
		|| s[i] == '\r' || s[i] == '\v' || s[i] == '\f' )
		i++;
	if (s[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (s[i] == '+')
		i++;
	while (s[i] >= '0' && s[i] <= '9')
	{
		number = number * 10 + s[i] - 48;
		i++;
	}
	return (number * sign);
}

/*
int main() {
    const char *str1 = "+12a45";
    const char *str2 = "z42";
    const char *str3 = "  -6789p";

    int num1 = ft_atoi(str1);
    int num2 = ft_atoi(str2);
    int num3 = ft_atoi(str3);

    printf("El número convertido de '%s' es: %d\n", str1, num1);  // Salida: 12
    printf("El número convertido de '%s' es: %d\n", str2, num2);  // Salida: 42
    printf("El número convertido de '%s' es: %d\n", str3, num3);// Salida: -6789

    return 0;
}*/
