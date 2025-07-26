/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 20:45:42 by yuliano           #+#    #+#             */
/*   Updated: 2025/07/26 23:35:32 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

t_data	*init_data()
{
	t_data *data;
	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->input = NULL;
	data->new_prompt = NULL;
	data->tree = NULL;
	return (data);

}


int main(void)
{
    t_data *data;

    data = init_data();

    while (1)
    {
        getcwd(data->prompt, sizeof(data->prompt));
        data->new_prompt = ft_strjoin(data->prompt, "$  ");
        data->input = readline(data->new_prompt);

        if (!data->input || strcmp(data->input, "exit") == 0)
        {
            ft_free(data);
            break;
        }

        if (*data->input)
            add_history(data->input);

        data->tree = parsepipe_tree(data->input);

        if (!data->tree)
            write(2, "Error\n", 6);
        else
            runcmd(data->tree, data);

        ft_free(data);
        rl_on_new_line();
    }

    rl_clear_history(); 
    free(data);
    return 0;
}



/*
void print_tree(t_tree_node *node, int level)
{
    if (!node)
        return;

    for (int i = 0; i < level; i++)
        printf("  ");

    if (is_node_type(node, "EXEC"))
    {
        t_exec *exec = (t_exec *)node->objeto;
        printf("[EXEC] %s\n", exec->argv[0]);
    }
    else if (is_node_type(node, "REDIR"))
    {
        t_redir *redir = (t_redir *)node->objeto;
        printf("[REDIR] fd=%d file=%s\n", redir->fd, redir->file);
    }

    print_tree(node->left, level + 1);
}

int main(void)
{
    char *input = "echo hola > file1 >> file2 < file3";
    printf("%s\n\n", input);
    t_tree_node *tree = parseexec_tree(input);

    printf("\n\033[1;32mÁrbol de ejecución generado:\033[0m\n");
    print_tree(tree, 0);

    // Aquí podrías liberar el árbol si tienes free_tree implementado
    return 0;
}
*/