<<<<<<< Updated upstream
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 22:33:01 by yuliano           #+#    #+#             */
/*   Updated: 2025/07/10 23:34:10 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
=======
#ifndef MINISHELL_H
# define MINISHELL_H
>>>>>>> Stashed changes

# include <ctype.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdbool.h>

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_EOF,
	TOKEN_ERROR
}	t_token_type;

<<<<<<< Updated upstream
#define EXEC  1   // Comando ejecutable simple
#define REDIR 2   // Comando con redirección (< o >)
#define PIPE  3   // Comando con pipe (|)
# define MAXARGS 15
=======
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef enum e_ast_type
{
	AST_COMMAND,
	AST_PIPE,
	AST_AND,
	ASYT_OR,
	AST_REDIRECT
}	t_ast_type;
>>>>>>> Stashed changes

// Estructura base de un comando
typedef struct s_cmd 
{
<<<<<<< Updated upstream
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
=======
	REDIR_IN,
	REDIT_OUT,
	REDIR_HEREDOC,
	REDIR_APPEND
}	t_redir_type;

typedef struct s_redirect
{
	t_redir_type		type;
	char				*file;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_ast_node
{
	t_ast_type	type;
	union
	{
		struct
		{
			char		**argv;
			t_redirect	*redirects;
		} s_command;
		struct
		{
			struct s_ast_node	*left;
			struct s_ast_node	*right;
		} s_binary;
	} u_data;
}	t_ast_node;

typedef struct s_lexer
{
	const char	*input;
	size_t		pos;
	size_t		len;
}	t_lexer;

typedef struct s_parser
{
	t_token	*current;
	t_token	*tokens;
	bool	error;
	char	*error_msg;
} t_parser;


int			ft_strlen(char *s);
char		*ft_strchr(char *str, char c);
char		*ft_strndup(char *str, size_t n);
int			ft_isalnum(char c);
static char	*extract_substring(const char *str, size_t start, size_t len);
char		*strjoin(char *s1, char *s2);
>>>>>>> Stashed changes
#endif