/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 22:47:01 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/03 17:25:59 by yuliano          ###   ########.fr       */
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
        "cd ./home .. .",
        "hola\"$PATH\"'$USER'",
        "echo \"que tal '$SALUDO'\"",
        NULL
    };
}
    */
