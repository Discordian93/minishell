/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 11:24:30 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/09 20:58:08 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_wrapped_in_quotes(const char *s)
{
    size_t len;

    if (!s)
        return 0;
    len = ft_strlen(s);
    if (len >= 2 && s[0] == '\'' && s[len - 1] == '\'')
        return 1;
    if (len >= 2 && s[0] == '"' && s[len - 1] == '"')
        return 1;
    return 0;
}

// Devuelve una copia del delimitador SIN las comillas externas si las tiene.
// Si no estaban, devuelve un strdup(s) normal. (Siempre retorna memoria nueva.)
char *strip_wrapping_quotes(const char *s)
{
    size_t len;

    if (!s)
        return NULL;
    len = ft_strlen(s);
    if (len >= 2 && ((s[0] == '\'' && s[len - 1] == '\'') ||
                     (s[0] == '"'  && s[len - 1] == '"')))
        return ft_strndup((char *)s + 1, len - 2);
    return ft_strdup(s);
}


// Expansión ligera para heredoc: expande $VAR pero NO quita comillas
char *expand_heredoc_line(char *line)
{
    char t[2];
    char *res = malloc(1);
    if (!res) return NULL;
    *res = '\0';

    while (*line)
    {
        if (*line == '$')
        {
            // Si NO hay nombre de variable válido detrás de '$', copiamos '$' tal cual
            if (!line[1] || !(ft_isalnum(line[1]) || line[1] == '_'))
            {
                t[0] = *line;
                t[1] = '\0';
                append(&res, t);
                line++;
                continue;
            }
            // Hay un nombre de variable: reutilizamos tu expand_var()
            expand_var(&line, &res);  // esta avanza 'line' internamente
        }
        else
        {
            t[0] = *line;
            t[1] = '\0';
            append(&res, t);
            line++;
        }
    }
    return res;
}

int handle_heredoc(char *delim)
{
    int   pipefd[2];
    char *line;
    char *cmp_delim;          // delimitador normalizado (sin comillas externas)
    int   quoted_delim;       // 1 si el delimitador venía entrecomillado
	char	*expanded;
    if (pipe(pipefd) == -1)
        panic("pipe failed\n");

    quoted_delim = is_wrapped_in_quotes(delim);
    cmp_delim    = strip_wrapping_quotes(delim);
    if (!cmp_delim)
        panic("malloc failed\n");

    while (1)
    {
        line = readline("> ");
        // EOF (Ctrl-D) → terminar
        if (!line)
            break;

        // ¿Llegó el delimitador? (comparación literal contra delimitador sin comillas)
        if (ft_strncmp(line, cmp_delim, ft_strlen(cmp_delim) + 1) == 0)
        {
            free(line);
            break;
        }
        /*si tiene comillas el delimitador, no hay expansion*/
        if (quoted_delim)
        {
            write(pipefd[WRITE_END], line, ft_strlen(line));
            write(pipefd[WRITE_END], "\n", 1);
        }
        else
        {
            // Delimitador sin comillas → SÍ hay expansión
            expanded = expand_heredoc_line(line);
            if (!expanded)
            {
                free(line);
                free(cmp_delim);
                panic("malloc failed\n");
            }
            write(pipefd[WRITE_END], expanded, ft_strlen(expanded));
            write(pipefd[WRITE_END], "\n", 1);
            free(expanded);
        }
        free(line);
    }

    free(cmp_delim);
    close(pipefd[WRITE_END]);          // cerramos el extremo de escritura
    return pipefd[READ_END];           // devolvemos el extremo de lectura
}
