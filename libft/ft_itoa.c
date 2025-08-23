/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 12:04:34 by ypacileo          #+#    #+#             */
/*   Updated: 2024/07/04 12:07:23 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	ft_counter(int num)
{
	int	count;

	count = 1;
	if (num < 0)
		count++;
	while (num / 10 != 0)
	{
		num = num / 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	int				len;
	char			*str;
	unsigned int	num;

	len = ft_counter(n);
	str = (char *) malloc(sizeof(char) * (len + 1));
	if (str == NULL)
		return (NULL);
	if (n == 0)
		str[0] = '0';
	str[len] = '\0';
	if (n < 0)
	{
		str[0] = '-';
		num = (unsigned int)(-(long)n);
	}
	else
		num = (unsigned int)n;
	while (num > 0)
	{
		str[--len] = (char)((num % 10) + '0');
		num = num / 10;
	}
	return (str);
}

/*	 #include <stdio.h>
int main()
{
	int n = -2147483648;
	printf("Digit count: %i\n", ft_counter(n));
	printf("Result: %s\n", ft_itoa(n));

	n = 0;
	printf("Digit count: %i\n", ft_counter(n));
	printf("Result: %s\n", ft_itoa(n));

	n = 123456;
	printf("Digit count: %i\n", ft_counter(n));
	printf("Result: %s\n", ft_itoa(n));

	n = -123456;
	printf("Digit count: %i\n", ft_counter(n));
	printf("Result: %s\n", ft_itoa(n));
}*/