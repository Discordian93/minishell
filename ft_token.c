/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 22:47:01 by yuliano           #+#    #+#             */
/*   Updated: 2025/07/13 00:24:59 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Cuenta cuántas palabras hay en la cadena,
// considerando como separadores: espacio, tabulación y salto de línea.
// También verifica que las comillas estén balanceadas.
int count_words(const char *s)
{
    int words = 0;
    int i = 0;
    int quote_single = 0;
    int quote_double = 0;

    while (s[i] != '\0')
    {
        // Saltar espacios
        while ((s[i] == ' ' || s[i] == '\t' || s[i] == '\n') && s[i] != '\0')
            i++;

        if (s[i] != '\0')
        {
            words++;
            if (s[i] == '\'')
            {   
                quote_single++;
                i++;
                while (s[i] != '\'' && s[i] != '\0')
                    i++;
                if (s[i] == '\0')
                    return (-1); // comilla simple no cerrada
                else
                    quote_single++;
                i++;
                
            }
            else if (s[i] == '"')
            {
                quote_double++;
                i++;
                while (s[i] != '"' && s[i] != '\0')
                    i++;
                if (s[i] == '\0')
                    return (-1); // comilla doble no cerrada
                else
                    quote_double++;
                i++;
                
            }
            else
            {
                while (s[i] != ' ' && s[i] != '\t' && s[i] != '\n' && s[i] != '\0')
                    i++;
            }
        }
    }

    if (quote_single % 2 != 0 || quote_double % 2 != 0)
        return (-1);

    return (words);
}

// Libera la memoria ya asignada si ocurre un error en medio del proceso
void free_split(char ***s, int index)
{
    int i = 0;
    while (i < index)
    {
        free((*s)[i]);
        i++;
    }
    free(*s);
    *s = NULL;
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


/*
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
        NULL
    };

    for (int i = 0; tests[i]; i++)
    {
        printf("\n==============================\n");
        printf("Prueba %d: %s\n", i + 1, tests[i]);
        printf("------------------------------\n");
        char **tokens = ft_token(tests[i]);
        print_tokens(tokens);

        // Liberar memoria
        if (tokens)
        {
            for (int j = 0; tokens[j]; j++)
                free(tokens[j]);
            free(tokens);
        }
    }

    return 0;
}
*/