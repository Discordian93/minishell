/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 15:12:23 by ypacileo          #+#    #+#             */
/*   Updated: 2024/06/30 19:18:32 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include <stdio.h>
#include <string.h>*/
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	ch;
	char	*ptr;

	ch = (char)c;
	ptr = (char *)s;
	while (*ptr != '\0')
		ptr++;
	while (*ptr != ch && ptr != s)
		ptr--;
	if (*ptr == ch)
		return (ptr);
	else
		return (0);
}

/*int main() {
    const char s[] = "this is a sample string";
    char *p1 = ft_strrchr(s, 'a');
	char *p2 = strrchr(s,'a');
    printf("%s\n", p1);
	printf("%s\n",p2);

    return 0;
}*/
