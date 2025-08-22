/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_aux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esteizag <esteizag@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:39:56 by esteizag          #+#    #+#             */
/*   Updated: 2025/08/22 13:39:57 by esteizag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Auxiliary function to create a "KEY=VALUE" string */
char	*create_env_string(char *key, char *value)
{
	char	*str;

	str = malloc(1);
	if (!str)
		return (NULL);
	str[0] = '\0';
	append(&str, key);
	append(&str, "=");
	append(&str, value);
	return (str);
}

/* Auxiliary function to free an entire environment array */
void	free_environ(char **env)
{
	size_t	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
		free(env[i++]);
	free(env);
}

/* Auxiliary function to free partial environment (for error handling) */
void	free_partial_environ(char **env, size_t count)
{
	size_t	i;

	if (!env)
		return ;
	i = 0;
	while (i < count)
		free(env[i++]);
	free(env);
}

/* Auxiliary function to count environment variables */
size_t	count_environ(char **env)
{
	size_t	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

char	**initialize_environ(char **env)
{
	size_t	i;
	char	**myenv;
	size_t	j;

	i = count_environ(env);
	myenv = malloc(sizeof(char *) * (i + 1));
	if (!myenv)
		return (NULL);
	j = 0;
	while (j < i)
	{
		myenv[j] = ft_strndup(env[j], ft_strlen(env[j]));
		if (!myenv[j])
		{
			free_partial_environ(myenv, j);
			return (NULL);
		}
		j++;
	}
	myenv[j] = NULL;
	return (myenv);
}
