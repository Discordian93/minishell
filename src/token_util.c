/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:37:58 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/22 13:49:17 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	quoted_token_size(const char *str, int *i)
{
	int		size;
	char	quote;

	size = 0;
	quote = str[*i];
	(*i)++;
	size++;
	while (str[*i] && str[*i] != quote)
	{
		(*i)++;
		size++;
	}
	if (str[*i] == quote)
	{
		(*i)++;
		size++;
	}
	return (size);
}

int	token_size(const char *str)
{
	int		size;
	int		i;
	int		op_len;

	size = 0;
	i = 0;
	op_len = check_operator(str, 0);
	if (op_len > 0)
		return (op_len);
	while (str[i] && str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
	{
		if (check_operator(str, i) > 0)
			break ;
		if (str[i] == '"' || str[i] == '\'')
			size += quoted_token_size(str, &i);
		else
		{
			i++;
			size++;
		}
	}
	return (size);
}

char	**init_token_array(const char *str, int *len)
{
	char	**s;

	*len = count_words(str);
	if (*len <= 0)
		return (NULL);
	s = malloc((*len + 1) * sizeof(char *));
	if (!s)
		return (NULL);
	return (s);
}

void	copy_token(char *dst, const char *src, int size)
{
	int	k;

	k = 0;
	while (k < size)
	{
		dst[k] = src[k];
		k++;
	}
	dst[k] = '\0';
}
