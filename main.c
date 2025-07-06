/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 22:35:17 by yuliano           #+#    #+#             */
/*   Updated: 2025/07/06 14:31:36 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

// panic(): Imprime mensaje de error y sale
void panic(char *msg) 
{
    perror(msg);
    exit(1);
}

// redircmd(): Crea un nodo REDIR para representar redirección
t_cmd *redircmd(t_cmd *subcmd, char *file, int mode, int fd) 
{
    t_redir *redir = malloc(sizeof(t_redir));
    if (!redir)
        panic("malloc failed");
    redir->type = REDIR;
    redir->cmd = subcmd;
    redir->file = file;
    redir->mode = mode;
    redir->fd = fd;
    return (t_cmd *)redir;
}

// parseexec(): Analiza comandos simples con redirecciones opcionales
t_cmd *parseexec(char *input) 
{
    char **token;
    t_cmd *cmd = NULL;
    t_exec *exec = malloc(sizeof(t_exec));
    int i,j,mode,fd;

    if (!exec)
        panic("malloc failed");
    exec->type = EXEC;
    j = 0;
    while(j < MAXARGS)
    {
        exec->argv[j] = NULL;
        j++;
    }

    token = ft_token(input);
    i = 0;
    j = 0;
    while (token[i] != NULL) 
    {
        if (ft_strncmp(token[i], ">", ft_strlen(token[i])) == 0 || ft_strncmp(token[i], "<", ft_strlen(token[i])) == 0) 
        {
            i++;
            if (!token[i])
                panic("Falta archivo después de redirección");
            if (ft_strncmp(token[i - 1], ">",ft_strlen(token[i - 1])) == 0) 
            {
                mode = O_WRONLY | O_CREAT | O_TRUNC;
                fd = 1;
            } else 
            {
                mode = O_RDONLY;
                fd = 0;
            }
            cmd = redircmd((t_cmd *)exec, token[i], mode, fd); // El EXEC se convierte en subcmd
        } else 
        {
            exec->argv[j++] = token[i]; //Agregamos solo si no es token de redirección
        }
        i++;
    }

    // Si no hubo redirección, el árbol es solo EXEC
    if (!cmd)
        cmd = (t_cmd *)exec;

    return cmd;
}

t_cmd   *parsepipe(char *input)
{
    char *pipe_pos;
    char *left_part;
    char *right_part;
    t_cmd *left;
    t_cmd *right;
    t_pipe *pipe;

    pipe_pos = ft_strchr(input, '|');
    if (pipe_pos == NULL)
    {
        printf("[DEBUG] caso base: %s\n", input);
        return parseexec(input);
    }

    printf ("[DEbUG] Dividiendo : %s\n", input);
    *pipe_pos = '\0';
    left_part = input;
    right_part = pipe_pos + 1;
    left = parseexec(left_part);
    right = parsepipe(right_part);
    //crear nodo PIPE
    pipe = malloc(sizeof(t_pipe));
    if (!pipe)
        panic("Error\n");
    pipe -> type = PIPE;
    pipe -> left = left;
    pipe -> right = right;
    printf("[DEBUG] Nodo pipe creado\n");
    return (t_cmd *)pipe;
}

int main() 
{
    char *input;
    t_cmd *tree;

    while (1) 
    {
        input = readline("minishell> ");

        if (!input || strcmp(input, "exit") == 0) {
            free(input);
            break;
        }

        if (*input)
            add_history(input);

        if ((tree = parsepipe(input)) == NULL) // Almacenamos el arbol para luego ejecutar
            write(2, "Error\n",7);
        // ☞ Ejecutar con runcmd(tree) cuando esté disponible

        // Liberar memoria si hace falta aquí (según implementación futura)
        free(input);
    }
    return 0;
}

