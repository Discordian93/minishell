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
	char	*c;
	size_t	len;

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
				
			}
	}
}