#ifndef MINISHELL_H
# define MINISHELL_H

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

typedef enum e_redir_type
{
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
void		*append(char **s1, char *s2);
void		*freeandnull(char *s);
int			is_variable_start(t_lexer *lexer);
#endif