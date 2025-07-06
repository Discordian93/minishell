/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 22:33:01 by yuliano           #+#    #+#             */
/*   Updated: 2025/07/06 12:56:46 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_SHELL
#define MINI_SHELL

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
# include "libft/libft.h"

#define EXEC  1   // Comando ejecutable simple
#define REDIR 2   // Comando con redirección (< o >)
#define PIPE  3   // Comando con pipe (|)
# define MAXARGS 15

// Estructura base de un comando
typedef struct s_cmd 
{
  int type;  // Tipo del comando (EXEC, PIPE, REDIR)
}	t_cmd;

// Nodo EXEC: Representa un comando simple (ej: ls -l)
typedef struct s_exec
{
  int type;
  char *argv[MAXARGS];
}	t_exec;

// Nodo REDIR: Representa una redirección (< o >)
typedef struct s_redir 
{
  int type;
  t_cmd *cmd;      // Subcomando sobre el que se aplica la redirección
  char *file;           // Nombre del archivo destino o fuente
  int mode;             // Modo de apertura (O_RDONLY, O_WRONLY, etc.)
  int fd;               // File descriptor a redirigir (0 = stdin, 1 = stdout)
}	t_redir;

// Nodo PIPE: Representa un pipe entre dos comandos (ej: ls | grep)
typedef struct s_pipe
{
  int type;
  t_cmd *left;     // Lado izquierdo del pipe
  t_cmd *right;    // Lado derecho del pipe
}	t_pipe;

char **ft_token(char *str);
#endif