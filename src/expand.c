/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 16:47:30 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/16 18:41:34 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_var(char **token, char **result)
{
	char	*varname;
	char	*varval;

	(*token)++;
	if (**token == '?')
	{
		(*token)++;
		expand_sig(result);
		return ;
	}
	varname = get_variable_name(token);
	varval = ft_getenv(varname);
	if (varval)
		append(result, varval);
	free(varname);
}

void	expand_quoted(char **token, char **result)
{
	char	quote;
	char	c[2];

	quote = **token;
	(*token)++;
	while (**token != quote)
	{
		if (quote == '"' && **token == '$')
			expand_var(token, result);
		else
		{
			c[0] = **token;
			c[1] = '\0';
			append(result, c);
			(*token)++;
		}
	}
	(*token)++;
}

char	*expand_token(char *token)
{
	char	*result;
	char	c[2];

	result = malloc(1);
	if (!result)
		return (NULL);
	*result = '\0';
	while (*token)
	{
		if (*token != '\'' && *token != '"' && *token != '$')
		{
			c[0] = *token;
			c[1] = '\0';
			append(&result, c);
			token++;
		}
		else if (*token == '\'' || *token == '"')
			expand_quoted(&token, &result);
		else if (*token == '$')
			expand_var(&token, &result);
	}
	return (result);
}

char	**expand_vars(char **tokens)
{
	char	**expanded;
	int		i;
	int		j;

	i = 0;
	while (tokens && tokens[i])
		i++;
	expanded = malloc(sizeof(char *) * (i + 1));
	if (!expanded)
		return (NULL);
	j = 0;
	while (j < i)
	{
		expanded[j] = expand_token(tokens[j]);
		j++;
	}
	expanded[j] = NULL;
	return (expanded);
}

char	*ft_strndup(char *str, size_t n)
{
	char	*dup;
	size_t	len;
	size_t	i;

	i = 0;
	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (n < len)
		len = n;
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	while (i < len)
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
