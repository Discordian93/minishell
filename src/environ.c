/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esteizag <esteizag@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:02:49 by esteizag          #+#    #+#             */
/*   Updated: 2025/08/22 13:02:51 by esteizag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*my_getenv(char *key, char **env)
{
	size_t	i;
	size_t	j;
	char	*split;

	i = count_environ(env);
	j = 0;
	while (j < i)
	{
		split = ft_strchr(env[j], '=');
		if (split)
		{
			*split = '\0';
			if (ft_strncmp(key, env[j], ft_strlen(key)) == 0
				&& ft_strlen(env[j]) == ft_strlen(key))
			{
				*split = '=';
				return (split + 1);
			}
			*split = '=';
		}
		j++;
	}
	return (NULL);
}

/* Auxiliary function to find environment variable index */
int	find_env_index(char *key, char **env)
{
	size_t	i;
	char	*split;

	i = 0;
	while (env[i])
	{
		split = ft_strchr(env[i], '=');
		if (split)
		{
			*split = '\0';
			if (ft_strncmp(key, env[i], ft_strlen(key)) == 0
				&& ft_strlen(env[i]) == ft_strlen(key))
			{
				*split = '=';
				return (i);
			}
			*split = '=';
		}
		i++;
	}
	return (-1);
}

/* Auxiliary function to copy environment excluding one index */
char	**copy_environ_except(char **env, size_t skip_index)
{
	size_t	i;
	size_t	j;
	size_t	count;
	char	**new_env;

	count = count_environ(env);
	new_env = malloc(sizeof(char *) * count);
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
	while (env[i])
	{
		if (i != skip_index)
		{
			new_env[j] = ft_strndup(env[i], ft_strlen(env[i]));
			if (!new_env[j])
				return (free_partial_environ(new_env, j), NULL);
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	return (new_env);
}

void	my_unsetenv(char *key, char ***env)
{
	int		index;
	char	**new_env;

	index = find_env_index(key, *env);
	if (index == -1)
		return ;
	new_env = copy_environ_except(*env, index);
	if (!new_env)
		exit(1);
	free_environ(*env);
	*env = new_env;
}

/*void	*handle_environ(char *key, size_t op)
{
	static size_t	initialized = 0;
	static char**	my_environ = NULL;
	extern char**	environ;

	if (initialized == 0)
	{
		my_environ = initialize_environ(environ);
		if (!my_environ)
			return (NULL);
		initialized = 1;
	}
	if (op == 1)
		return ((char *) my_getenv(key, my_environ));
	else if (op == 3)
		return ((char **) my_environ);
	else
		return (NULL);
}*/