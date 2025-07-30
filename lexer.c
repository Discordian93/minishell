#include "minishell.h"

t_lexer *lexer_init(const char *input)
{
	t_lexer	*lexer;

	lexer = malloc(sizeof(lexer));
	if(!lexer)
		return (NULL);
	lexer->input =input;
	lexer->pos = 0;
	lexer->len = ft_strlen(input);
	return (lexer);
}

static void	skip_whitespace(t_lexer *lexer)
{
	while(lexer->pos < lexer->len &&
	ft_strchr(" \t\n", lexer->input[lexer->pos]))
		lexer->pos++;
}

static bool	is_word_char(char c)
{
	return (c && !ft_strchr(" \t\n|<>&()", c));
}

static char	*read_quoted_string(t_lexer *lexer, char quote)
{
	size_t	start;
	char	*result;

	lexer->pos++;
	start = lexer->pos;
	while (lexer->pos < lexer->len && lexer->input[lexer->pos] != quote)
		lexer->pos++;
	if (lexer->pos >= lexer->len)
		return (NULL);
	result = ft_strndup(lexer->input + start, lexer->pos - start);
	lexer->pos++;
	if (!result)
		return (NULL);
	return (result);
}

static char *expand_single_var(const char *str, size_t *pos, size_t len)
{
	size_t	start;
	size_t	var_len;
	char	*var_name;
	char	*value;

	(*pos)++;
	if (*pos < len && str[*pos] == '?')
	{
		(*pos)++;
		return (ft_strndup("0", 1));
	}
	start = *pos;
	while (*pos < len && (ft_isalnum(str[*pos]) || str[*pos] == '_'))
		(*pos)++;
	var_len = *pos - start;
	if (var_len == 0)
		return (ft_strndup("$", 1));
	var_name = extract_substring(str, start, var_len);
	if (!var_name)
		return (NULL);
	value = getenv(var_name);
	free(var_name);
	if (value)
		return (ft_strndup(value, ft_strlen(value)));
	else
		return (ft_strndup("", 0));
}

static char	*expand_variables(const char *str)
{
	char	*result;
	size_t	i;
	char	c[2];
	size_t	len;
	char	*expanded;

	result = malloc(1);
	if (!result)
		return (NULL);
	*result = '\0';
	i = 0;
	len = (ft_strlen(str));
	while (i < len)
	{
		if (str[i] == '$'
			&& (i+ 1) < len 
			&& (str[i + 1] == '?'
			|| ft_isalnum(str[i + 1])
			|| str[i + 1] == '_'))
			{
				expanded = expand_single_var(str , &i, len);
				if(expanded)
				{
					append(&result, expanded);
					free(expanded);
				}
			}
			else
			{
				c[0] = str[i++];
				c[1] = '\0';
				append(&result, c);
			}
	}
	return (result);
}


static int	handle_quoted_part(char **word, t_lexer *lexer)
{
	char	*quoted_content;
	char	*expanded_content;
	char	quote_type;

	quote_type = lexer->input[lexer->pos];
	quoted_content = read_quoted_string(lexer, quote_type);
	if (!quoted_content)
		return (-1);
	if (quote_type == '"')
	{
		expanded_content = expand_variables(quoted_content);
		free(quoted_content);
		if (!expanded_content)
			return (-1);
		quoted_content = expanded_content;
	}
	append(word, quoted_content);
	free(quoted_content);
	return (0);
}

static void	handle_unquoted_expansion(char **word, t_lexer *lexer)
{
	char	*expanded_part;

	expanded_part = expand_single_var(lexer->input, &(lexer->pos), lexer->len);
	if (expanded_part)
	{
		append(word, expanded_part);
		free(expanded_part);
	}
}

static int	build_word_segment(char **word, t_lexer *lexer)
{
	char	c;
	char	tmp[2];

	c = lexer->input[lexer->pos];
	if (c == '\'' || c == '"')
	{
		if (handle_quoted_part(word, lexer) == -1)
			return (-1);
	}
	else if (is_variable_start(lexer))
		handle_unquoted_expansion(word, lexer);
	else if (c && !ft_strchr(" \t\n|<>&()", c))
	{
		tmp[0] = lexer->input[lexer->pos++];
		tmp[1] = '\0';
		append(word, tmp);
	}
	else
		return (1);
	
	return (0);
}

static char	*read_word(t_lexer *lexer)
{
	char	*word;
	size_t	start_pos;
	int		status;

	word = malloc(1);
	if (!word)
		return (NULL);
	*word = '\0';
	start_pos = lexer->pos;
	while (lexer->pos < lexer->len)
	{
		status = build_word_segment(&word, lexer);
		if (status == 1)
			break;
		if (status == -1)
			return (freeandnull(word));
	}
	if (lexer->pos == start_pos)
		return (freeandnull(word));
	return (word);
}

static t_token	*create_token(t_token_type type, char *value)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if (!token)
		return NULL;
	token->type = type;
	token->value = value;
	token->next = NULL;
	return token;
}

t_token	*handle_double_symbol(t_lexer *lexer)
{
	char	c;
	char	*next;

	c = lexer->input[lexer->pos];
	lexer->pos += 2;
	if (c == '&')
		return (create_token(TOKEN_AND, ft_strndup("&&", 2)));
	else if (c == '|')
		return (create_token(TOKEN_OR, ft_strndup("||", 2)));
	else if (c == '<')
		return (create_token(TOKEN_HEREDOC, ft_strndup("<<", 2)));
	else if (c == '>')
		return (create_token(TOKEN_APPEND, ft_strndup(">>", 2)));
	else
		return (NULL);
}

t_token	*handle_single_symbol(t_lexer *lexer)
{
	char	c;
	char	*next;

	c = lexer->input[lexer->pos];
	lexer->pos++;
	if (c == '|')
		return (create_token(TOKEN_PIPE, ft_strndup("|", 1)));
	if (c == '<')
		return (create_token(TOKEN_REDIRECT_IN, ft_strndup("<", 1)));
	if (c == '>')
		return (create_token(TOKEN_REDIRECT_OUT, ft_strndup(">", 1)));
	if (c == '(')
		return (create_token(TOKEN_LPAREN, ft_strndup("(", 1)));
	if (c == ')')
		return (create_token(TOKEN_RPAREN, ft_strndup(")", 1)));
	else
		return (NULL);
}

t_token	*handle_word(t_lexer *lexer)
{
	char *word;

	word = read_word(lexer);
	if (!word)
		return (create_token(TOKEN_ERROR, NULL));
	else
		return (create_token(TOKEN_WORD, word));
}

static t_token	*get_next_token(t_lexer *lexer)
{
	char	c;
	char	next;
	t_token	*token;
	char	*word;

	skip_whitespace(lexer);
	if (lexer->pos >= lexer->len)
		return create_token(TOKEN_EOF, NULL);
	c = lexer->input[lexer->pos];
	if(lexer->pos + 1 < lexer->len)
	{
		next = lexer->input[lexer->pos + 1];
		if (c == next && ft_strchr("|<>&", c))
			return (handle_double_symbol(lexer));
	}
	if (ft_strchr("|<>()", c))
		return (handle_single_symbol(lexer));
	return (handle_word(lexer));
}

t_token	*lexer_tokenize(t_lexer *lexer)
{
	t_token	*head;
	t_token	*tail;
	t_token	*token;

	head = NULL;
	tail = NULL;
	while (1)
	{
		token = get_next_token(lexer);
		if (!token)
			break;
		if (token->type == TOKEN_ERROR)
		{
			token_list_destroy(head);
			free(token);
			return (NULL);
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
	return (head);
}

void	lexer_destroy(t_lexer *lexer)
{
	if (lexer)
		free(lexer);
}

void	token_list_destroy(t_token *tokens)
{
	t_token	*next;

	while (tokens)
	{
		next = tokens->next;
		if (tokens->value)
			free(tokens->value);
		free(tokens);
		tokens = next;
	}
}