/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esteizag <esteizag@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 15:05:58 by esteizag          #+#    #+#             */
/*   Updated: 2025/08/22 15:06:01 by esteizag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append(char **s1, char *s2)
{
	char	*appended;

	appended = ft_strjoin(*s1, s2);
	if (appended)
	{
		free(*s1);
		*s1 = appended;
	}
	else
		exit(1);
}

void	expand_sig(char **result)
{
	char	*sig;

	sig = ft_itoa(status);
	if (!sig)
		return ;
	append(result, sig);
	free(sig);
}

char	*get_variable_name(char **token)
{
	char	*varname;
	char	c_str[2];

	c_str[1] = '\0';
	varname = malloc(1);
	if (!varname)
		exit(1);
	*varname = '\0';
	while (ft_isalnum(**token) || **token == '_')
	{
		c_str[0] = **token;
		append(&varname, c_str);
		(*token)++;
	}
	return (varname);
}
