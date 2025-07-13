/* minishell.h - Main header file */
#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdbool.h>
# include <ctype.h>

/* Token types for lexical analysis */
typedef enum e_token_type
{
    TOKEN_WORD,           // Command or argument
    TOKEN_PIPE,           // |
    TOKEN_REDIRECT_IN,    // <
    TOKEN_REDIRECT_OUT,   // >
    TOKEN_HEREDOC,        // <<
    TOKEN_APPEND,         // >>
    TOKEN_AND,            // &&
    TOKEN_OR,             // ||
    TOKEN_LPAREN,         // (
    TOKEN_RPAREN,         // )
    TOKEN_EOF,            // End of input
    TOKEN_ERROR           // Lexical error
}   t_token_type;

/* Token structure */
typedef struct s_token
{
    t_token_type        type;
    char                *value;
    struct s_token      *next;
}   t_token;

/* AST node types */
typedef enum e_ast_type
{
    AST_COMMAND,          // Simple command
    AST_PIPE,             // Pipeline
    AST_AND,              // && operator
    AST_OR,               // || operator
    AST_REDIRECT          // Redirection
}   t_ast_type;

/* Redirection types */
typedef enum e_redir_type
{
    REDIR_IN,             // <
    REDIR_OUT,            // >
    REDIR_HEREDOC,        // <<
    REDIR_APPEND          // >>
}   t_redir_type;

/* Redirection structure */
typedef struct s_redirect
{
    t_redir_type        type;
    char                *file;
    struct s_redirect   *next;
}   t_redirect;

/* AST node structure */
typedef struct s_ast_node
{
    t_ast_type          type;
    union
    {
        struct {
            char        **argv;     // Command arguments
            t_redirect  *redirects; // Redirections
        } command;
        struct {
            struct s_ast_node *left;
            struct s_ast_node *right;
        } binary;
    } data;
}   t_ast_node;

/* Lexer state structure */
typedef struct s_lexer
{
    const char  *input;
    size_t      pos;
    size_t      len;
}   t_lexer;

/* Parser state structure */
typedef struct s_parser
{
    t_token     *current;
    t_token     *tokens;
    bool        error;
    char        *error_msg;
}   t_parser;

/* Lexer functions */
t_lexer     *lexer_init(const char *input);
t_token     *lexer_tokenize(t_lexer *lexer);
void        lexer_destroy(t_lexer *lexer);
void        token_list_destroy(t_token *tokens);

/* Parser functions */
t_parser    *parser_init(t_token *tokens);
t_ast_node  *parser_parse(t_parser *parser);
void        parser_destroy(t_parser *parser);
void        ast_destroy(t_ast_node *node);

/* Utility functions */
char        *ft_strdup(const char *s);
char        *ft_strndup(const char *s, size_t n);
void        *ft_calloc(size_t count, size_t size);

#endif /* MINISHELL_H */