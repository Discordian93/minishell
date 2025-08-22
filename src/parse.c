/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:17:43 by ypacileo          #+#    #+#             */
/*   Updated: 2025/08/22 12:18:10 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Inicializa una estructura t_exec para almacenar los argumentos del comando.
t_exec	*initialize_exec(void)
{
	t_exec	*exec;
	int		j;

	exec = malloc(sizeof(t_exec));
	if (!exec)
		panic("malloc failed\n", EXIT_FAILURE);
	j = 0;
	while (j < MAXARGS)
	{
		exec->argv[j] = NULL;
		j++;
	}
	return (exec);
}

// Construye el árbol de ejecución para un comando simple con redirecciones.
t_tree	*build_exec_tree(t_exec *exec, char **token)
{
	t_tree	*root_exec;
	t_tree	*current;
	int		i;
	int		j;

	root_exec = create_tree_node((void *)exec, "EXEC");
	current = root_exec;
	i = 0;
	j = 0;
	while (token[i] != NULL)
	{
		if (is_redirection(token[i]))
			handle_redirection(&current, token, &i);
		else
			exec->argv[j++] = expand_token(token[i]);
		i++;
	}
	return (root_exec);
}

// Parsea comandos simples con múltiples redirecciones encadenadas
// y construye un árbol de ejecución para el comando.
t_tree	*parseexec_tree(char *input)
{
	char	**token;
	t_exec	*exec;
	t_tree	*root_exec;

	exec = initialize_exec();
	if (!exec)
		return (NULL);
	token = ft_token(input);
	if (!token)
		return (NULL);
	root_exec = build_exec_tree(exec, token);
	free_split(&token, count_split(token));
	return (root_exec);
}
