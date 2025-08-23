/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 12:16:00 by ypacileo          #+#    #+#             */
/*   Updated: 2024/06/23 13:10:27 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include <stdio.h>
#include <string.h>*/
#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*str;

	str = 0;
	if (s1 != 0 && set != 0)
	{
		start = 0;
		end = ft_strlen(s1);
		while (s1[start] != '\0' && ft_strchr(set, s1[start]))
			start++;
		while (end > start && ft_strchr(set, s1[end - 1]))
			end--;
		str = malloc(sizeof(char) * (end - start + 1));
		if (!str)
			return (0);
		else
			ft_strlcpy(str, &s1[start], end - start + 1);
	}
	return (str);
}

/* int	main()
{
	char s1[] = "123456";
	char set[] = "256";

	printf("%s", ft_strtrim(s1,set));
}*/