/* test_parser.c - Test program for the parser */
#include "minishell.h"

/* Print tokens for debugging */
static void print_tokens(t_token *tokens)
{
    printf("Tokens:\n");
    while (tokens)
    {
        printf("  Type: ");
        switch (tokens->type)
        {
            case TOKEN_WORD: printf("WORD"); break;
            case TOKEN_PIPE: printf("PIPE"); break;
            case TOKEN_REDIRECT_IN: printf("REDIRECT_IN"); break;
            case TOKEN_REDIRECT_OUT: printf("REDIRECT_OUT"); break;
            case TOKEN_HEREDOC: printf("HEREDOC"); break;
            case TOKEN_APPEND: printf("APPEND"); break;
            case TOKEN_AND: printf("AND"); break;
            case TOKEN_OR: printf("OR"); break;
            case TOKEN_LPAREN: printf("LPAREN"); break;
            case TOKEN_RPAREN: printf("RPAREN"); break;
            case TOKEN_EOF: printf("EOF"); break;
            case TOKEN_ERROR: printf("ERROR"); break;
        }
        if (tokens->value)
            printf(", Value: '%s'", tokens->value);
        printf("\n");
        tokens = tokens->next;
    }
}

/* Print AST for debugging */
static void print_ast_indent(t_ast_node *node, int indent)
{
    if (!node)
        return;

    for (int i = 0; i < indent; i++)
        printf("  ");

    switch (node->type)
    {
        case AST_COMMAND:
            printf("COMMAND:");
            for (int i = 0; node->data.command.argv[i]; i++)
                printf(" %s", node->data.command.argv[i]);
            
            t_redirect *redir = node->data.command.redirects;
            while (redir)
            {
                printf(" [");
                switch (redir->type)
                {
                    case REDIR_IN: printf("<"); break;
                    case REDIR_OUT: printf(">"); break;
                    case REDIR_HEREDOC: printf("<<"); break;
                    case REDIR_APPEND: printf(">>"); break;
                }
                printf(" %s]", redir->file);
                redir = redir->next;
            }
            printf("\n");
            break;

        case AST_PIPE:
            printf("PIPE\n");
            print_ast_indent(node->data.binary.left, indent + 1);
            print_ast_indent(node->data.binary.right, indent + 1);
            break;

        case AST_AND:
            printf("AND\n");
            print_ast_indent(node->data.binary.left, indent + 1);
            print_ast_indent(node->data.binary.right, indent + 1);
            break;

        case AST_OR:
            printf("OR\n");
            print_ast_indent(node->data.binary.left, indent + 1);
            print_ast_indent(node->data.binary.right, indent + 1);
            break;

        case AST_REDIRECT:
            printf("REDIRECT\n");
            break;
    }
}

static void print_ast(t_ast_node *node)
{
    printf("\nAST:\n");
    print_ast_indent(node, 0);
}

/* Test the parser with various inputs */
int main(void)
{
    const char *test_cases[] = {
        // Simple commands
        "ls -la",
        "echo hello world",
        
        // Pipes
        "cat file.txt | grep pattern | wc -l",
        
        // Redirections
        "echo hello > output.txt",
        "cat < input.txt > output.txt",
        "cat << EOF >> file.txt",
        
        // Logical operators
        "make && ./program",
        "test -f file || echo 'file not found'",
        
        // Complex with parentheses
        "(cd /tmp && ls) || echo failed",
        "echo start && (cat file | grep test || echo notfound) && echo end",
        
        // Quotes and variables
        "echo \"Hello $USER\"",
        "echo 'Hello $USER'",
        "echo $HOME/Documents",
        "echo \"Exit status: $?\"",
        
        // Mixed complex command
        "(ls -la | grep '.txt' && echo found) || (echo not found > error.log)",
        
        NULL
    };

    for (int i = 0; test_cases[i]; i++)
    {
        printf("\n=== Test case %d: %s ===\n", i + 1, test_cases[i]);
        
        // Tokenize
        t_lexer *lexer = lexer_init(test_cases[i]);
        if (!lexer)
        {
            printf("Failed to create lexer\n");
            continue;
        }
        
        t_token *tokens = lexer_tokenize(lexer);
        lexer_destroy(lexer);
        
        if (!tokens)
        {
            printf("Tokenization failed\n");
            continue;
        }
        
        print_tokens(tokens);
        
        // Parse
        t_parser *parser = parser_init(tokens);
        if (!parser)
        {
            printf("Failed to create parser\n");
            token_list_destroy(tokens);
            continue;
        }
        
        t_ast_node *ast = parser_parse(parser);
        
        if (parser->error)
        {
            printf("Parse error: %s\n", parser->error_msg);
        }
        else if (ast)
        {
            print_ast(ast);
            ast_destroy(ast);
        }
        else
        {
            printf("Empty AST\n");
        }
        
        parser_destroy(parser);
        token_list_destroy(tokens);
    }
    
    return 0;
}