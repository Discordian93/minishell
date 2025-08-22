/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esteizag <esteizag@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:04:46 by esteizag          #+#    #+#             */
/*   Updated: 2025/08/22 13:04:47 by esteizag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

char	*ft_getenv(char *key)
{
	return ((char *) handle_environ(key, NULL, 1));
}

void	ft_setenv(char *key, char *value)
{
	handle_environ(key, value, 2);
	return ;
}

char	**env(void)
{
	return ((char **) handle_environ(NULL, NULL, 3));
}
