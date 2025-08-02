#include "minishell.h"

int	ft_strlen(char *s)
{
	int	count;

	count = 0;
	while (*s)
	{
		count++;
		s++;
	}
	return (count);
}

char	*ft_strchr(char *str, char c)
{
	while (*str)
	{
		if (*str == c)
			return (str);
		str++;
	}
	if (*str == (char)c)
		return (str);
	return (NULL);
}

char	*ft_strndup(char *str, size_t n)
{
	char	*dup;
	size_t	len;
	size_t	i;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (n < len)
		len = n;
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	while (i < len)
		dup[i] = str[i++];
	dup[i] = '\0';
	return (dup);
}

int	ft_isalnum(char c)
{
	return ((c >= 48 && c <= 57)
		|| (c >= 65 && c <= 90)
		|| (c >= 97 && c <= 122));
}

static char	*extract_substring(const char *str, size_t start, size_t len)
{
	char	*substring;
	size_t	i;

	substring = malloc(len + 1);
	if (!substring)
		return (NULL);
	i = 0;
	while (i < len)
		substring[i] = str[start + i++];
	substring[i] = '\0';
	return (substring);
}

char	*strjoin(char *s1, char *s2)
{
	int		len1;
	int		len2;
	char	*joined;
	size_t	i;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	joined = malloc(len1 + len2 + 1);
	if (!joined)
		return (NULL);
	i = 0;
	while (i < len1)
		joined[i] = s1[i++];
	i = 0;
	while (i < len2)
		joined[i + len1] = s2[i++];
	joined[len1 + len2] = '\0';
	return (joined);
}

void	*append(char **s1, char *s2)
{
	char	*appended;

	appended = strjoin(*s1, s2);
	if (appended)
	{
		free(*s1);
		*s1 = appended;
	}
	else
		exit(1);
}

void	*freeandnull(char *s)
{
	free(s);
	return (NULL);
}

int	is_variable_start(t_lexer *lexer)
{
	char	next_char;

	if (lexer->input[lexer->pos] == '$' && (lexer->pos + 1) < lexer->len)
	{
		next_char = lexer->input[lexer->pos + 1];
		if (next_char == '?'
			|| ft_isalnum(next_char)
			|| next_char == '_')
		{
			return (1);
		}
	}
	return (0);
}