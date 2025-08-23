/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 22:47:01 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/22 13:39:20 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_token(const char *str)
{
	int		i;
	int		size;
	int		len;
	char	**s;

	s = init_token_array(str, &len);
	if (!s)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str = skip_spaces(str);
		size = token_size(str);
		s[i] = malloc(size + 1);
		if (!s[i])
		{
			free_split(&s, i);
			return (NULL);
		}
		copy_token(s[i], str, size);
		str += size;
		i++;
	}
	s[i] = NULL;
	return (s);
}
