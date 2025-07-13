/* lexer.c - Tokenizer implementation */
#include "minishell.h"
#include <ctype.h>

/* Initialize lexer with input string */
t_lexer *lexer_init(const char *input)
{
    t_lexer *lexer;

    lexer = ft_calloc(1, sizeof(t_lexer));
    if (!lexer)
        return NULL;
    lexer->input = input;
    lexer->pos = 0;
    lexer->len = strlen(input);
    return lexer;
}

/* Skip whitespace characters */
static void skip_whitespace(t_lexer *lexer)
{
    while (lexer->pos < lexer->len && 
           (lexer->input[lexer->pos] == ' ' || 
            lexer->input[lexer->pos] == '\t' ||
            lexer->input[lexer->pos] == '\n'))
        lexer->pos++;
}

/* Check if character is a word character */
static bool is_word_char(char c)
{
    return c && !strchr(" \t\n|<>&()", c);
}

/* Read a quoted string */
static char *read_quoted_string(t_lexer *lexer, char quote)
{
    size_t start;
    char *result;

    lexer->pos++; // Skip opening quote
    start = lexer->pos;
    
    while (lexer->pos < lexer->len && lexer->input[lexer->pos] != quote)
        lexer->pos++;
    
    if (lexer->pos >= lexer->len)
        return NULL; // Unclosed quote
    
    result = ft_strndup(lexer->input + start, lexer->pos - start);
    lexer->pos++; // Skip closing quote
    
    return result;
}

/* Expand environment variables in a string */
static char *expand_variables(const char *str)
{
    char *result = malloc(1024); // Simplified - should be dynamic
    size_t i = 0, j = 0;
    
    if (!result)
        return NULL;
    
    while (str[i])
    {
        if (str[i] == '$' && str[i + 1])
        {
            size_t var_start = ++i;
            while (str[i] && (isalnum(str[i]) || str[i] == '_'))
                i++;
            
            char var_name[256];
            strncpy(var_name, str + var_start, i - var_start);
            var_name[i - var_start] = '\0';
            
            char *value = getenv(var_name);
            if (value)
            {
                strcpy(result + j, value);
                j += strlen(value);
            }
        }
        else
            result[j++] = str[i++];
    }
    result[j] = '\0';
    return result;
}

/* Read a word token (handles quotes and variable expansion) */
static char *read_word(t_lexer *lexer)
{
    char buffer[4096] = {0}; // Simplified - should be dynamic
    size_t buf_pos = 0;
    
    while (lexer->pos < lexer->len)
    {
        char c = lexer->input[lexer->pos];
        
        if (c == '\'' || c == '"')
        {
            char *quoted = read_quoted_string(lexer, c);
            if (!quoted)
                return NULL; // Unclosed quote
            
            if (c == '"')
            {
                // Expand variables in double quotes
                char *expanded = expand_variables(quoted);
                free(quoted);
                quoted = expanded;
            }
            
            strcpy(buffer + buf_pos, quoted);
            buf_pos += strlen(quoted);
            free(quoted);
        }
        else if (c == '$' && lexer->input[lexer->pos + 1] == '?')
        {
            // Handle $? (exit status)
            buffer[buf_pos++] = '0'; // Placeholder
            lexer->pos += 2;
        }
        else if (c == '$' && lexer->input[lexer->pos + 1] && 
                 (isalnum(lexer->input[lexer->pos + 1]) || 
                  lexer->input[lexer->pos + 1] == '_'))
        {
            // Handle environment variables
            size_t start = ++lexer->pos;
            while (lexer->pos < lexer->len && 
                   (isalnum(lexer->input[lexer->pos]) || 
                    lexer->input[lexer->pos] == '_'))
                lexer->pos++;
            
            char var_name[256];
            strncpy(var_name, lexer->input + start, lexer->pos - start);
            var_name[lexer->pos - start] = '\0';
            
            char *value = getenv(var_name);
            if (value)
            {
                strcpy(buffer + buf_pos, value);
                buf_pos += strlen(value);
            }
        }
        else if (is_word_char(c))
        {
            buffer[buf_pos++] = c;
            lexer->pos++;
        }
        else
            break;
    }
    
    if (buf_pos == 0)
        return NULL;
    
    return ft_strdup(buffer);
}

/* Create a new token */
static t_token *create_token(t_token_type type, char *value)
{
    t_token *token;

    token = ft_calloc(1, sizeof(t_token));
    if (!token)
        return NULL;
    token->type = type;
    token->value = value;
    token->next = NULL;
    return token;
}

/* Get next token from input */
static t_token *get_next_token(t_lexer *lexer)
{
    char c;

    skip_whitespace(lexer);
    
    if (lexer->pos >= lexer->len)
        return create_token(TOKEN_EOF, NULL);
    
    c = lexer->input[lexer->pos];
    
    // Check for two-character operators
    if (lexer->pos + 1 < lexer->len)
    {
        char next = lexer->input[lexer->pos + 1];
        
        if (c == '&' && next == '&')
        {
            lexer->pos += 2;
            return create_token(TOKEN_AND, ft_strdup("&&"));
        }
        else if (c == '|' && next == '|')
        {
            lexer->pos += 2;
            return create_token(TOKEN_OR, ft_strdup("||"));
        }
        else if (c == '<' && next == '<')
        {
            lexer->pos += 2;
            return create_token(TOKEN_HEREDOC, ft_strdup("<<"));
        }
        else if (c == '>' && next == '>')
        {
            lexer->pos += 2;
            return create_token(TOKEN_APPEND, ft_strdup(">>"));
        }
    }
    
    // Check for single-character operators
    switch (c)
    {
        case '|':
            lexer->pos++;
            return create_token(TOKEN_PIPE, ft_strdup("|"));
        case '<':
            lexer->pos++;
            return create_token(TOKEN_REDIRECT_IN, ft_strdup("<"));
        case '>':
            lexer->pos++;
            return create_token(TOKEN_REDIRECT_OUT, ft_strdup(">"));
        case '(':
            lexer->pos++;
            return create_token(TOKEN_LPAREN, ft_strdup("("));
        case ')':
            lexer->pos++;
            return create_token(TOKEN_RPAREN, ft_strdup(")"));
        default:
            break;
    }
    
    // Read word token
    char *word = read_word(lexer);
    if (!word)
        return create_token(TOKEN_ERROR, NULL);
    
    return create_token(TOKEN_WORD, word);
}

/* Tokenize entire input */
t_token *lexer_tokenize(t_lexer *lexer)
{
    t_token *head = NULL;
    t_token *tail = NULL;
    t_token *token;

    while (1)
    {
        token = get_next_token(lexer);
        if (!token)
            break;
        
        if (token->type == TOKEN_ERROR)
        {
            token_list_destroy(head);
            free(token);
            return NULL;
        }
        
        if (!head)
        {
            head = token;
            tail = token;
        }
        else
        {
            tail->next = token;
            tail = token;
        }
        
        if (token->type == TOKEN_EOF)
            break;
    }
    
    return head;
}

/* Destroy lexer */
void lexer_destroy(t_lexer *lexer)
{
    if (lexer)
        free(lexer);
}

/* Destroy token list */
void token_list_destroy(t_token *tokens)
{
    t_token *next;

    while (tokens)
    {
        next = tokens->next;
        if (tokens->value)
            free(tokens->value);
        free(tokens);
        tokens = next;
    }
}