/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_environ.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esteizag <esteizag@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:25:54 by esteizag          #+#    #+#             */
/*   Updated: 2025/08/22 14:26:11 by esteizag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Auxiliary function to set or unset environment variable */
void	modify_environ(char *key, char *value, size_t op, char ***env)
{
	if (op == 2)
		my_setenv(key, value, env);
	else if (op == 4)
		my_unsetenv(key, env);
}

/*
	op 1 is getenv
	op 2 is setenv
	op 3 is getting entire env
	op 4 is unsetenv
*/
void	*handle_environ(char *key, char *value, size_t op)
{
	static size_t	initialized = 0;
	static char		**my_environ = NULL;
	extern char		**environ;

	if (initialized == 0)
	{
		my_environ = initialize_environ(environ);
		if (!my_environ)
			return (NULL);
		initialized = 1;
	}
	if (op == 1)
		return (my_getenv(key, my_environ));
	else if (op == 2 || op == 4)
	{
		modify_environ(key, value, op, &my_environ);
		return (NULL);
	}
	else if (op == 3)
		return ((char **)my_environ);
	else
		return (NULL);
}
