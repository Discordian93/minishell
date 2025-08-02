/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 22:47:01 by yuliano           #+#    #+#             */
/*   Updated: 2025/07/26 23:50:49 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Salta los espacios, tabulaciones y saltos de línea en la cadena.
// Devuelve el índice del primer carácter no separador.
int skip_separators(const char *s, int i)
{
    while ((s[i] == ' ' || s[i] == '\t' || s[i] == '\n') && s[i] != '\0')
        i++;
    return i;
}

// Avanza el índice hasta el final de la palabra entre comillas.
// Devuelve el índice después de la comilla de cierre o -1 si no está balanceada.
int skip_quoted_word(const char *s, int i)
{
    char start_quote = s[i++];
    while (s[i] != start_quote && s[i] != '\0')
        i++;
    if (s[i] == '\0')
        return (-1);
    return (i + 1);
}

// Avanza el índice hasta el final de la palabra sin comillas.
// Devuelve el índice del primer separador o fin de cadena.
int skip_unquoted_word(const char *s, int i)
{
    while (s[i] != ' ' && s[i] != '\t' && s[i] != '\n' && s[i] != '\0')
        i++;
    return (i);
}

// Cuenta cuántas palabras hay en la cadena,
// considerando como separadores: espacio, tabulación y salto de línea.
// También verifica que las comillas estén balanceadas.
int count_words(const char *s)
{
    int words = 0;
    int i = 0;
    int next;

    while (s[i] != '\0')
    {
        i = skip_separators(s, i);

        if (s[i] != '\0')
        {
            words++;
            if (s[i] == '\'' || s[i] == '\"')
            {
                next = skip_quoted_word(s, i);
                if (next == -1)
                    return -1;
                i = next;
            }
            else
            {
                i = skip_unquoted_word(s, i);
            }
        }
    }

    return (words);
}


// Split de entrada considerando comillas simples y dobles
char **ft_token(const char *str)
{
    int i = 0, j, size, len;
    char **s;

    if (!str)
        return (NULL);

    len = count_words(str);
    if (len == -1)
        return (NULL);

    s = (char **)malloc((len + 1) * sizeof(char *));
    if (!s)
        return (NULL);

    while (i < len)
    {
        while ((*str == ' ' || *str == '\t' || *str == '\n') && *str != '\0')
            str++;

        size = 0;
        if (*str == '\'')
        {
            str++;
            while (str[size] != '\'' && str[size] != '\0')
                size++;
        }
        else if (*str == '"')
        {
            str++;
            while (str[size] != '"' && str[size] != '\0')
                size++;
        }
        else
        {
            while (str[size] != ' ' && str[size] != '\t' && str[size] != '\n' && str[size] != '\0')
                size++;
        }

        s[i] = (char *)malloc((size + 1) * sizeof(char));
        if (!s[i])
        {
            free_split(&s, i);
            return (NULL);
        }

        j = 0;
        while (j < size)
        {
            s[i][j] = str[j];
            j++;
        }
        s[i][j] = '\0';

        str += size;
        if (*str == '\'' || *str == '"')
            str++;

        i++;
    }

    s[i] = NULL;
    return (s);
}

char	*strjoin(char *s1, char *s2)
{
	size_t		len1;
	size_t		len2;
	char	*joined;
	size_t	i;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	joined = malloc(len1 + len2 + 1);
	if (!joined)
		return (NULL);
	i = 0;
	while (i < len1)
	{
            joined[i] = s1[i];
            i++;
    }
	i = 0;
	while (i < len2)
	{
        joined[i + len1] = s2[i];
        i++;
    }
	joined[len1 + len2] = '\0';
	return (joined);
}

char	*ft_strndup(char *str, size_t n)
{
	char	*dup;
	size_t	len;
	size_t	i;

    i= 0;
	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (n < len)
		len = n;
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	while (i < len)
	{
        dup[i] = str[i];
        i++;
    }
	dup[i] = '\0';
	return (dup);
}

void	append(char **s1, char *s2)
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

void expand_var(char **token, char **result)
{
    char    *varname;
    char    *varval;
    char c[2];

    (*token)++;
    varname = malloc(1);
    if (!varname)
        exit(1);
    *varname = '\0';
    while (ft_isalnum(**token) || **token == '_')
    {
        c[0] = **token;
        c[1] = '\0';
        append(&varname, c);
        (*token)++;
    }
    varval = getenv(varname);
    free(varname);
    if (varval)
		append(result, varval);
}

void expand_quoted(char **token, char **result)
{
    char    quote;
    char    c[2];

    quote = **token;
    (*token)++;
    while (**token != quote)
    {
        if (quote == '"' && **token == '$')
            expand_var(token, result);
        else
        {
            c[0] = **token;
            c[1] = '\0';
            append(result, c);
            (*token)++;
            printf("avanza...\n");
        }
    }
    (*token)++;
}

char *expand_token(char *token)
{
    char    *result;
    char    c[2];

    result = malloc(1);
    if (!result)
        return (NULL);
    *result = '\0';
    while(*token)
    {
        if (*token != '\'' && *token != '"' && *token != '$')
        {
            c[0] = *token;
            c[1] = '\0';
            append(&result, c);
            token++;
        }
        else if (*token == '\'' || *token == '"')
            expand_quoted(&token, &result);
        else if (*token == '$')
            expand_var(&token, &result);
    }
    return (result);
}

char    **expand_vars(char **tokens)
{   
    char    **expanded;
    int     i;
    int     j;

    i = 0;
    while (tokens && tokens[i])
        i++;
    expanded = malloc(sizeof(char*) * (i +1));
    if (!expanded)
        return (NULL);
    j = 0;
    while (j < i)
    {
        expanded[j] = expand_token(tokens[j]);
        j++;
    }
    expanded[j] = NULL;
    return (expanded);
}



#include <stdio.h>
#include <stdlib.h>


void print_tokens(char **tokens)
{
    int i = 0;
    if (!tokens)
    {
        printf("NULL (Error de comillas)\n");
        return;
    }
    while (tokens[i])
    {
        printf("Token %d: [%s]\n", i, tokens[i]);
        i++;
    }
}



int main(void)
{
    char *tests[] = {
        "echo Hola Mundo",                     // sin comillas
        "echo 'Hola Mundo'",                  // comillas simples
        "echo \"Hola Mundo\"",                // comillas dobles
        "echo \"Hola $USER\"",                // dobles comillas con variable
        "echo \"hola '$USER'\"",                  // comillas simples con metacaracteres
        "echo 'Hola",                         // comilla simple no cerrada
        "echo \"Hola",                        // comilla doble no cerrada
        "'Hola mundo'",                       // solo comillas simples
        "\"Hola mundo\"",                     // solo comillas dobles
        "    'Hola   mundo   '     ",      // espacios exteriores con comillas
        "ls -l | grep 'main.c'",              // mezcla con metacaracteres
        "'Esto  es una \"prueba\" comillas simples'",
        "cd ./home .. .",
        "hola\"$PATH\"'$USER'",
        "\"que tal '$SALUDO'\"",
        NULL
    };

    char **expanded;

    for (int i = 0; tests[i]; i++)
    {
        printf("\n==============================\n");
        printf("Prueba %d: %s\n", i + 1, tests[i]);
        printf("------------------------------\n");
        char **tokens = ft_token(tests[i]);
        print_tokens(tokens);
        expanded = expand_vars(tokens);
        printf("expandidas: \n");
        print_tokens(expanded); 
        // Liberar memoria
        if (tokens)
        {
            for (int j = 0; tokens[j]; j++)
               { free(tokens[j]);
                free(expanded[j]);
                }
            free(tokens);
        }
    }

    return 0;
}
