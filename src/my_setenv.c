/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esteizag <esteizag@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:29:46 by esteizag          #+#    #+#             */
/*   Updated: 2025/08/22 13:29:48 by esteizag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_key_match(char *key, char *env_entry)
{
	char	*split;
	int		match;

	split = ft_strchr(env_entry, '=');
	if (!split)
		return (0);
	*split = '\0';
	match = (ft_strncmp(key, env_entry, ft_strlen(key)) == 0
			&& ft_strlen(env_entry) == ft_strlen(key));
	*split = '=';
	return (match);
}

static void	update_env_value(char **environ, size_t idx, char *key, char *value)
{
	char	*new_str;

	new_str = create_env_string(key, value);
	if (!new_str)
		exit(1);
	free(environ[idx]);
	environ[idx] = new_str;
}

void	my_setenv(char *key, char *value, char ***env)
{
	size_t	i;
	char	**environ;

	environ = *env;
	i = 0;
	while (environ[i])
	{
		if (env_key_match(key, environ[i]))
		{
			update_env_value(environ, i, key, value);
			return ;
		}
		i++;
	}
	add_to_environ(env, key, value);
}
