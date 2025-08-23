/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_count.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:34:55 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/22 13:52:34 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_operator(const char *s, int i)
{
	if (!s[i])
		return (0);
	if (s[i] == '>' && s[i + 1] == '>')
		return (2);
	if (s[i] == '<' && s[i + 1] == '<')
		return (2);
	if (s[i] == '>' || s[i] == '<' || s[i] == '|')
		return (1);
	return (0);
}

const char	*skip_spaces(const char *s)
{
	while (*s == ' ' || *s == '\t' || *s == '\n')
		s++;
	return (s);
}

int	skip_quoted(const char *s, int i)
{
	char	quote;

	quote = s[i];
	i++;
	while (s[i] && s[i] != quote)
		i++;
	if (s[i] == quote)
		i++;
	return (i);
}

int	skip_word(const char *s, int i)
{
	char	quote;
	int		op_len;

	op_len = check_operator(s, i);
	if (op_len > 0)
		return (i + op_len);
	while (s[i] != ' ' && s[i] != '\t' && s[i] != '\n' && s[i] != '\0')
	{
		if (check_operator(s, i) > 0)
			break ;
		if (s[i] == '"' || s[i] == '\'')
		{
			quote = s[i++];
			while (s[i] != quote && s[i] != '\0')
				i++;
			if (s[i] == '\0')
				return (-1);
		}
		if (s[i] != '\0')
			i++;
	}
	return (i);
}

int	count_words(const char *s)
{
	int	words;
	int	i;
	int	next_i;

	words = 0;
	i = 0;
	while (s[i] != '\0')
	{
		while (s[i] == ' ' || s[i] == '\t' || s[i] == '\n')
			i++;
		if (s[i] != '\0')
		{
			words++;
			next_i = skip_word(s, i);
			if (next_i == -1)
				return (-1);
			i = next_i;
		}
	}
	return (words);
}
