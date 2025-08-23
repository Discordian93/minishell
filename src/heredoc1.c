/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:55:37 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/23 13:55:48 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Objetivo: Lee líneas desde la entrada estándar hasta 
	encontrar el delimitador especificado.
	Si el delimitador está entre comillas, no expande variables. 
	Si no, expande $VAR.
	Escribe las líneas leídas en el extremo d
	e escritura de un pipe y retorna el extremo de lectura.
*/
void	write_line_to_pipe(int fd, const char *line)
{
	size_t	len;

	len = ft_strlen(line);
	write(fd, line, len);
	write(fd, "\n", 1);
}

/*
	Objetivo: Procesa una línea de heredoc,
	expandiéndola si es necesario y escribiéndola en el pipe.
*/
void	process_heredoc_line(int fd, char *line, int quoted_delim)
{
	char	*expanded;

	expanded = NULL;
	if (quoted_delim)
		write_line_to_pipe(fd, line);
	else
	{
		expanded = expand_heredoc_line(line);
		if (!expanded)
			panic("malloc failed\n", EXIT_FAILURE);
		write_line_to_pipe(fd, expanded);
		free(expanded);
	}
}
