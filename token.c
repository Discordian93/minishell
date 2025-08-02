/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 11:27:09 by ypacileo          #+#    #+#             */
/*   Updated: 2025/08/02 19:37:36 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <stdio.h> // para print_tokens

// Cuenta cuántas palabras hay en la cadena considerando comillas como agrupadores
// Si hay una comilla sin cerrar, devuelve -1 para evitar errores posteriores
int count_words(char *s)
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

// Libera los tokens previamente reservados hasta el índice dado
void free_split(char ***s, int index)
{
    int i = 0;
    while (i < index)
        free((*s)[i++]);
    free(*s);
    *s = NULL;
}

// Divide una cadena en tokens respetando comillas y espacios
char **ft_token(char *str)
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

// Imprime los tokens obtenidos por ft_token
void print_tokens(char **tokens)
{
    int i = 0;
    if (!tokens)
    {
        printf("(null)\n");
        return;
    }
    while (tokens[i])
    {
        printf("token[%d]: %s\n", i, tokens[i]);
        i++;
    }
}

// Main para probar ft_token con varios inputs
int main(void)
{
    // Casos de prueba con comillas, redirecciones y errores
    char *tests[] = {
        "echo hola mundo",
        "echo 'hola mundo'",
        "echo \"hola mundo\"",
        "ls -l | grep main > salida.txt",
        "'comilla simple' y \"comilla doble\"",
        "sin cerrar \"comilla",
        "q\"$PATH hola\"",
        NULL
    };

    int i = 0;
    while (tests[i])
    {
        printf("\nTest %d: %s\n", i + 1, tests[i]);
        char **tokens = ft_token(tests[i]);
        print_tokens(tokens);
        if (tokens)
        {
            int j = 0;
            while (tokens[j])
                free(tokens[j++]);
            free(tokens);
        }
        i++;
    }
    return 0;
}
