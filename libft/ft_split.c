/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 13:20:36 by ypacileo          #+#    #+#             */
/*   Updated: 2024/07/13 13:11:45 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

// Función para contar las subcadenas
static size_t	ft_count_substr(char const *s, char c)
{
	size_t	count;
	size_t	i;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			count++;
			while (s[i] != c && s[i])
				i++;
		}
		else
		{
			i++;
		}
	}
	return (count);
}

// Función para liberar memoria
static void	ft_free_dou_array(char **array, int i)
{
	while (i >= 0)
	{
		free(array[i]);
		i--;
	}
	free(array);
}

// Función para llenar el array con las subcadenas
static char	**ft_fill(char const *s, char c, char **array, int count)
{
	int	i;
	int	start;
	int	end;

	i = 0;
	end = 0;
	while (i < count)
	{
		start = end;
		while (s[start] == c)
			start++;
		end = start;
		while (s[end] != c && s[end])
			end++;
		array[i] = ft_substr(s, start, (end - start));
		if (array[i] == NULL)
		{
			ft_free_dou_array(array, i - 1);
			return (NULL);
		}
		i++;
	}
	array[i] = NULL;
	return (array);
}

// Función principal
char	**ft_split(char const *s, char c)
{
	char	**array;
	size_t	count;

	if (!s)
		return (NULL);
	count = ft_count_substr(s, c);
	array = (char **)malloc((count + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	if (!ft_fill(s, c, array, count))
	{
		return (NULL);
	}
	return (array);
}

/*	#include <stdio.h>
 int  main(void)
{
   char    *s = "   cara de culo";
   char    c = ' ';
   size_t  count;
   char    **tab = ft_split(s, c);
   size_t  i;


   i = 0;
   count = ft_count_substr(s, c);
   printf("s[0]: %c\n", s[0]);
   printf("COUNT en main: %zu\n", count);
   while (i < count)
   {
       printf("%s\n", tab[i]);
       i++;
   }
}*/