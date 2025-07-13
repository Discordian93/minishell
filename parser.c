/* parser.c - Parser implementation */
#include "minishell.h"

/* Initialize parser */
t_parser *parser_init(t_token *tokens)
{
    t_parser *parser;

    parser = ft_calloc(1, sizeof(t_parser));
    if (!parser)
        return NULL;
    parser->tokens = tokens;
    parser->current = tokens;
    parser->error = false;
    parser->error_msg = NULL;
    return parser;
}

/* Advance to next token */
static void advance(t_parser *parser)
{
    if (parser->current && parser->current->type != TOKEN_EOF)
        parser->current = parser->current->next;
}

/* Check if current token matches type */
static bool check(t_parser *parser, t_token_type type)
{
    if (!parser->current)
        return false;
    return parser->current->type == type;
}

/* Match and consume token */
static bool match(t_parser *parser, t_token_type type)
{
    if (check(parser, type))
    {
        advance(parser);
        return true;
    }
    return false;
}

/* Set parser error */
static void set_error(t_parser *parser, const char *msg)
{
    parser->error = true;
    parser->error_msg = ft_strdup(msg);
}

/* Create AST node */
static t_ast_node *create_ast_node(t_ast_type type)
{
    t_ast_node *node;

    node = ft_calloc(1, sizeof(t_ast_node));
    if (!node)
        return NULL;
    node->type = type;
    return node;
}

/* Parse redirections */
static t_redirect *parse_redirections(t_parser *parser)
{
    t_redirect *head = NULL;
    t_redirect *tail = NULL;

    while (parser->current && 
           (check(parser, TOKEN_REDIRECT_IN) ||
            check(parser, TOKEN_REDIRECT_OUT) ||
            check(parser, TOKEN_HEREDOC) ||
            check(parser, TOKEN_APPEND)))
    {
        t_redirect *redir = ft_calloc(1, sizeof(t_redirect));
        if (!redir)
            return head;

        if (check(parser, TOKEN_REDIRECT_IN))
        {
            redir->type = REDIR_IN;
            advance(parser);
        }
        else if (check(parser, TOKEN_REDIRECT_OUT))
        {
            redir->type = REDIR_OUT;
            advance(parser);
        }
        else if (check(parser, TOKEN_HEREDOC))
        {
            redir->type = REDIR_HEREDOC;
            advance(parser);
        }
        else if (check(parser, TOKEN_APPEND))
        {
            redir->type = REDIR_APPEND;
            advance(parser);
        }

        if (!check(parser, TOKEN_WORD))
        {
            set_error(parser, "Expected filename after redirection");
            free(redir);
            return head;
        }

        redir->file = ft_strdup(parser->current->value);
        advance(parser);

        if (!head)
        {
            head = redir;
            tail = redir;
        }
        else
        {
            tail->next = redir;
            tail = redir;
        }
    }

    return head;
}

/* Count command arguments */
static int count_args(t_parser *parser)
{
    t_token *token = parser->current;
    int count = 0;

    while (token && token->type == TOKEN_WORD)
    {
        count++;
        token = token->next;
        // Stop if we hit a redirection or operator
        if (token && (token->type == TOKEN_REDIRECT_IN ||
                      token->type == TOKEN_REDIRECT_OUT ||
                      token->type == TOKEN_HEREDOC ||
                      token->type == TOKEN_APPEND ||
                      token->type == TOKEN_PIPE ||
                      token->type == TOKEN_AND ||
                      token->type == TOKEN_OR ||
                      token->type == TOKEN_RPAREN))
            break;
    }

    return count;
}

/* Parse simple command */
static t_ast_node *parse_command(t_parser *parser)
{
    t_ast_node *node;
    int argc;
    int i = 0;

    // Handle empty command
    if (!check(parser, TOKEN_WORD))
        return NULL;

    node = create_ast_node(AST_COMMAND);
    if (!node)
        return NULL;

    // Count arguments
    argc = count_args(parser);
    node->data.command.argv = ft_calloc(argc + 1, sizeof(char *));
    if (!node->data.command.argv)
    {
        free(node);
        return NULL;
    }

    // Collect arguments
    while (parser->current && parser->current->type == TOKEN_WORD && i < argc)
    {
        node->data.command.argv[i++] = ft_strdup(parser->current->value);
        advance(parser);
    }
    node->data.command.argv[i] = NULL;

    // Parse redirections
    node->data.command.redirects = parse_redirections(parser);

    return node;
}

/* Forward declarations for recursive descent */
static t_ast_node *parse_logical_or(t_parser *parser);

/* Parse parenthesized expression */
static t_ast_node *parse_paren_expr(t_parser *parser)
{
    t_ast_node *node;

    if (!match(parser, TOKEN_LPAREN))
        return parse_command(parser);

    node = parse_logical_or(parser);
    
    if (!match(parser, TOKEN_RPAREN))
    {
        set_error(parser, "Expected ')' after expression");
        ast_destroy(node);
        return NULL;
    }

    return node;
}

/* Parse pipeline */
static t_ast_node *parse_pipeline(t_parser *parser)
{
    t_ast_node *left;
    t_ast_node *right;
    t_ast_node *pipe_node;

    left = parse_paren_expr(parser);
    if (!left)
        return NULL;

    while (match(parser, TOKEN_PIPE))
    {
        right = parse_paren_expr(parser);
        if (!right)
        {
            set_error(parser, "Expected command after pipe");
            ast_destroy(left);
            return NULL;
        }

        pipe_node = create_ast_node(AST_PIPE);
        if (!pipe_node)
        {
            ast_destroy(left);
            ast_destroy(right);
            return NULL;
        }

        pipe_node->data.binary.left = left;
        pipe_node->data.binary.right = right;
        left = pipe_node;
    }

    return left;
}

/* Parse logical AND */
static t_ast_node *parse_logical_and(t_parser *parser)
{
    t_ast_node *left;
    t_ast_node *right;
    t_ast_node *and_node;

    left = parse_pipeline(parser);
    if (!left)
        return NULL;

    while (match(parser, TOKEN_AND))
    {
        right = parse_pipeline(parser);
        if (!right)
        {
            set_error(parser, "Expected command after &&");
            ast_destroy(left);
            return NULL;
        }

        and_node = create_ast_node(AST_AND);
        if (!and_node)
        {
            ast_destroy(left);
            ast_destroy(right);
            return NULL;
        }

        and_node->data.binary.left = left;
        and_node->data.binary.right = right;
        left = and_node;
    }

    return left;
}

/* Parse logical OR */
static t_ast_node *parse_logical_or(t_parser *parser)
{
    t_ast_node *left;
    t_ast_node *right;
    t_ast_node *or_node;

    left = parse_logical_and(parser);
    if (!left)
        return NULL;

    while (match(parser, TOKEN_OR))
    {
        right = parse_logical_and(parser);
        if (!right)
        {
            set_error(parser, "Expected command after ||");
            ast_destroy(left);
            return NULL;
        }

        or_node = create_ast_node(AST_OR);
        if (!or_node)
        {
            ast_destroy(left);
            ast_destroy(right);
            return NULL;
        }

        or_node->data.binary.left = left;
        or_node->data.binary.right = right;
        left = or_node;
    }

    return left;
}

/* Main parse function */
t_ast_node *parser_parse(t_parser *parser)
{
    t_ast_node *ast;

    ast = parse_logical_or(parser);
    
    if (!parser->error && parser->current && 
        parser->current->type != TOKEN_EOF)
    {
        set_error(parser, "Unexpected token");
        ast_destroy(ast);
        return NULL;
    }

    return ast;
}

/* Destroy parser */
void parser_destroy(t_parser *parser)
{
    if (parser)
    {
        if (parser->error_msg)
            free(parser->error_msg);
        free(parser);
    }
}

/* Destroy AST recursively */
void ast_destroy(t_ast_node *node)
{
    if (!node)
        return;

    switch (node->type)
    {
        case AST_COMMAND:
            if (node->data.command.argv)
            {
                for (int i = 0; node->data.command.argv[i]; i++)
                    free(node->data.command.argv[i]);
                free(node->data.command.argv);
            }
            
            t_redirect *redir = node->data.command.redirects;
            while (redir)
            {
                t_redirect *next = redir->next;
                if (redir->file)
                    free(redir->file);
                free(redir);
                redir = next;
            }
            break;

        case AST_PIPE:
        case AST_AND:
        case AST_OR:
            ast_destroy(node->data.binary.left);
            ast_destroy(node->data.binary.right);
            break;

        default:
            break;
    }

    free(node);
}