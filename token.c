/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_antiguo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 11:27:09 by ypacileo          #+#    #+#             */
/*   Updated: 2025/08/02 18:44:40 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "minishell.h"
//#include "minishell.h"
#include <stdlib.h>

// Cuenta cuántas palabras hay en la cadena,
// considerando como separadores: espacio, tabulación y salto de línea.
int count_words(char *s)
{
    int words = 0;
    int i = 0;

    while (s[i] != '\0')
    {    
        // Saltar espacios o separadores
        while ((s[i] == ' ' || s[i] == '\t' || s[i] == '\n') && s[i] != '\0' && s[i] != '\'')
            i++;
        // Si encontramos una palabra, la contamos
        if (s[i] != '\0')
        {
            if (s[i] == '\'')
            {
                words++;
                i++;
                while (s[i] != '\'' && s[i] != '\0')
                    i++;
                if (s[i] == '\0')
                    return (-1); // comilla sin cerrar
                else
                    i++;
            }
            else if (s[i] == '"')
            {
                words++;
                i++;
                while (s[i] != '"' && s[i] != '\0')
                    i++;
                if (s[i] == '\0')
                    return (-1); // comilla sin cerrar
                else
                    i++;
            }
            else
            {
                words++;
                while (s[i] != ' ' && s[i] != '\t' && s[i] != '\n' && s[i] != '\0')
                    i++;
            }
        }
    }
    return (words);
}

// Libera la memoria ya asignada si ocurre un error en medio del proceso
void free_split(char ***s, int index)
{
    int i = 0;
    while (i < index) // liberamos solo lo que ya fue asignado
    {
        free((*s)[i]);
        i++;
    }
    free(*s);
    *s = NULL;
}

// Funcíon principal que hace el split por espacios, tabs o saltos de línea
char **ft_token(char *str)
{
    int i = 0, j, size, len;
    char **s;

    if (!str)
        return (NULL);

    len = count_words(str);
    if (len == -1)
        return (NULL); // comilla sin cerrar

    s = (char **)malloc((len + 1) * sizeof(char *)); // +1 para el NULL final
    if (!s)
        return (NULL);

    while (i < len)
    {
        // Saltar separadores
        while ((*str == ' ' || *str == '\t' || *str == '\n') && *str != '\0' && *str != '\'')
            str++;

        // Calcular el tamaño de la palabra actual
        size = 0;
        if (*str == '\'')
        {
            size++; // saltar la comilla de apertura
            while(str[size] != '\'' && str[size] != '\0')
                size++;
            if (str[size] == '\'')
                size++;
        }
        else if (*str == '"')
        {
            size++;
            while (str[size] != '"' && str[size] != '\0')
                size++;
            if(str[size] == '"')
                size++;
        }
        else
        {
            while (str[size] != ' ' && str[size] != '\t' &&
                str[size] != '\n' && str[size] != '\0')
                size++;
        }

        // Reservar espacio para la palabra + '\0'
        s[i] = (char *)malloc((size + 1) * sizeof(char));
        if (!s[i])
        {
            free_split(&s, i); // liberamos hasta i - 1 con i < index
            return (NULL);
        }

        // Copiar la palabra
        j = 0;
        while (j < size)
        {
            s[i][j] = str[j];
            j++;
        }
        s[i][j] = '\0'; // terminador de string

        // Avanzar el puntero en str para seguir procesando
        str += size;
        /*if (*str == '\'' || *str == '"')
            str++; */
    }

    s[i] = NULL; // último elemento del array es NULL
    return (s);
}



#include <stdio.h>


int main(void)
{
    char *test = "echo \"hola $USER\" > txt";
    char **result = ft_token(test);

    if (!result)
    {
        printf("Error en ft_split\n");
        return (1);
    }

    int i = 0;
    while (result[i])
    {
        printf("Palabra %d: \"%s\"\n", i + 1, result[i]);
        i++;
    }

    // Usamos tu función personalizada para liberar memoria
    free_split(&result, i);

    return (0);
}
