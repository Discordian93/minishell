/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 17:30:10 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/17 20:54:11 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_input(char *str)
{
	int i;
	
	if(str[0] == '\0')
		return (1);
		
	i = 0;
	while(str[i] != '\0')
	{
		if (str[i] != ' ')
			return (0);
		i++;
	}

	return (1);
}


char	*my_ft_strchr(const char *s, int c)
{
	char		ch;
	char		*ptr;
	char		quote;
	
	if (!s)
		return (NULL);

	ch = (char)c;
	ptr = (char *)s;
	while (*ptr != ch && *ptr != '\0')
	{
		if (*ptr == '\'' || *ptr == '"')
		{
			quote = *ptr++;
			while(*ptr != quote && *ptr != '\0')
				ptr++;		
		}
		ptr++;
	}
		
	if (*ptr == ch)
		return (ptr);
	else
		return (NULL);
}



/*int main()
{
	char *input = "echo '|' |";
	if (my_ft_strchr(input, '|'))
		printf("encontrado\n");
	else
		printf("no encontrado\n");
	return (0);
}*/