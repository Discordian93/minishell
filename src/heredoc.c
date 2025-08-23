/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 11:24:30 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/22 15:09:36 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_wrapped_in_quotes(const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = ft_strlen(s);
	if (len >= 2 && s[0] == '\'' && s[len - 1] == '\'')
		return (1);
	if (len >= 2 && s[0] == '"' && s[len - 1] == '"')
		return (1);
	return (0);
}

char	*strip_wrapping_quotes(const char *s)
{
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	if (len >= 2 && ((s[0] == '\'' && s[len - 1] == '\'')
			|| (s[0] == '"' && s[len - 1] == '"')))
		return (ft_strndup((char *)s + 1, len - 2));
	return (ft_strdup(s));
}

void	append_char(char **res, char c)
{
	char	t[2];

	t[0] = c;
	t[1] = '\0';
	append(res, t);
}

char	*expand_heredoc_line(char *line)
{
	char	*res;

	res = malloc(1);
	if (!res)
		return (NULL);
	*res = '\0';
	while (*line)
	{
		if (*line == '$')
		{
			if (!line[1] || !(ft_isalnum(line[1]) || line[1] == '_'))
			{
				append_char(&res, *line);
				line++;
				continue ;
			}
			expand_var(&line, &res);
		}
		else
		{
			append_char(&res, *line);
			line++;
		}
	}
	return (res);
}
