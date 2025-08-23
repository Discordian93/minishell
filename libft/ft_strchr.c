/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 16:32:22 by ypacileo          #+#    #+#             */
/*   Updated: 2024/06/30 19:04:58 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include <stdio.h>
#include <string.h>*/

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	if (!s)
		return (NULL);
	char		ch;
	char		*ptr;

	ch = (char)c;
	ptr = (char *)s;
	while (*ptr != ch && *ptr != '\0')
		ptr++;
	if (*ptr == ch)
		return (ptr);
	else
		return (NULL);
}

/*int main() {
    const char *str = "la casa";
    char *p1 = ft_strchr(str, 'a');
	char *p2 = strchr(str,'a');

    printf("%s\n",p1);
	printf("%s\n",p2);
}*/
