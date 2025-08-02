/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 22:47:01 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/02 19:49:36 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Cuenta cuántas palabras hay en la cadena considerando comillas como agrupadores
// Si hay una comilla sin cerrar, devuelve -1 para evitar errores posteriores
int count_words(const char *s)
{
    int words = 0;
    int i = 0;

    while (s[i] != '\0')
    {
        // Saltar espacios iniciales
        while (s[i] == ' ' || s[i] == '\t' || s[i] == '\n')
            i++;
        if (s[i] != '\0')
        {
            words++;
            // Avanzar sobre la palabra actual
            while (s[i] != ' ' && s[i] != '\t' && s[i] != '\n' && s[i] != '\0')
            {
                // Si encuentra una comilla, buscar la de cierre
                if (s[i] == '"' || s[i] == '\'')
                {
                    char quote = s[i++];
                    while (s[i] != quote && s[i] != '\0')
                        i++;
                    if (s[i] == '\0')
                        return -1; // comilla sin cerrar
                }
                if (s[i] != '\0')
                    i++;
            }
        }
    }
    return words;
}
/*
// Libera los tokens previamente reservados hasta el índice dado
void free_split(char ***s, int index)
{
    int i = 0;
    while (i < index)
        free((*s)[i++]);
    free(*s);
    *s = NULL;
}*/

// Divide una cadena en tokens respetando comillas y espacios
char **ft_token(const char *str)
{
    int i = 0, j, size, len;
    char **s;

    if (!str)
        return NULL;

    len = count_words(str);
    if (len <= 0)
        return NULL;

    s = malloc((len + 1) * sizeof(char *));
    if (!s)
        return NULL;

    while (i < len)
    {
        // Saltar espacios antes del token
        while (*str == ' ' || *str == '\t' || *str == '\n')
            str++;

        size = 0;
        // Medir tamaño del token respetando comillas internas
        while (str[size] && str[size] != ' ' && str[size] != '\t' && str[size] != '\n')
        {
            if (str[size] == '"' || str[size] == '\'')
            {
                char quote = str[size++];
                while (str[size] != quote && str[size] != '\0')
                    size++;
                if (str[size] == quote)
                    size++;
            }
            else
                size++;
        }

        // Reservar memoria para el token e incluir terminador nulo
        s[i] = malloc(size + 1);
        if (!s[i])
        {
            free_split(&s, i);
            return NULL;
        }

        // Copiar el contenido del token desde str
        j = 0;
        while (j < size)
        {
            s[i][j] = str[j];
            j++;
        }
        s[i][j] = '\0';
        str += size;
        i++;
    }
    s[i] = NULL;
    return s;
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
    //printf("DEBUG: Building variable name...\n");
    while (ft_isalnum(**token) || **token == '_')
    {
        c[0] = **token;
        c[1] = '\0';
        append(&varname, c);
        (*token)++;
    }
    //printf("DEBUG: Variable name: '%s'\n", varname);
    varval = getenv(varname);
    //printf("DEBUG: Variable value: '%s'\n", varval ? varval : "(null)");
    if (varval)
        append(result, varval);
    free(varname);
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
            //printf("avanza...\n");
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
    //printf("DEBUG: expand_token starting with: '%s'\n", token);
    while(*token)
    {
        //printf("DEBUG: Current char: '%c'\n", *token);
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
    //printf("DEBUG: expand_token result: '%s'\n", result);
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



/*#include <stdio.h>
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
        "echo \"que tal '$SALUDO'\"",
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

    */
