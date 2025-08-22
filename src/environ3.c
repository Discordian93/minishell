/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esteizag <esteizag@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:07:18 by esteizag          #+#    #+#             */
/*   Updated: 2025/08/22 13:07:23 by esteizag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	ft_unsetenv(char *key)
{
	handle_environ(key, NULL, 4);
	return ;
}

void	export_one(char *s)
{
	char	*split;

	if (!s)
		return ;
	split = ft_strchr(s, '=');
	if (!split)
		return ;
	*split = '\0';
	if (*s == '\0' || ft_isdigit(*s))
	{
		write(2, s, ft_strlen(s));
		write(2, " is not a valid identifier.\n", ft_strlen(" is not a valid identifier.\n"));
	};
	ft_setenv(s, split + 1);
	*split = '=';
}

void	export(char **s)
{
	s++;
	while (*s)
	{
		export_one(*s);
		s++;
	}
}

void	ft_env(void)
{
	char	**my_env;

	my_env = env();
	if (my_env)
	{
		while (*my_env)
		{
			printf("%s\n", *my_env);
			my_env++;
		}
	}
}

void	ft_unset(char **args)
{
	if (args)
	{
		args++;
		while (*args)
		{
			ft_unsetenv(*args);
			args++;
		}
	}
}
