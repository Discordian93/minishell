/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_environ.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esteizag <esteizag@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:22:47 by esteizag          #+#    #+#             */
/*   Updated: 2025/08/22 13:22:50 by esteizag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**alloc_new_env(char **env)
{
	size_t	count;
	char	**new_env;

	count = count_environ(env);
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		exit(1);
	return (new_env);
}

static int	copy_env_vars(char **src, char **dst)
{
	size_t	i;

	i = 0;
	while (src[i])
	{
		dst[i] = ft_strndup(src[i], ft_strlen(src[i]));
		if (!dst[i])
		{
			free_partial_environ(dst, i);
			return (0);
		}
		i++;
	}
	return (1);
}

void	add_to_environ(char ***envdir, char *key, char *value)
{
	char	**env;
	char	**myenv;
	size_t	i;

	env = *envdir;
	myenv = alloc_new_env(env);
	if (!copy_env_vars(env, myenv))
		return ;
	i = count_environ(env);
	myenv[i] = create_env_string(key, value);
	if (!myenv[i])
	{
		free_partial_environ(myenv, i);
		return ;
	}
	myenv[i + 1] = NULL;
	free_environ(env);
	*envdir = myenv;
}
