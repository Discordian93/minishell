/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 20:45:42 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/20 23:22:27 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_data	*init_data(void)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->input = NULL;
	data->new_prompt = NULL;
	data->tree = NULL;
	ft_memset(data->prompt, 0, sizeof(data->prompt));
	return (data);
}

void	draw_prompt(t_data *data)
{
	char	*input;
	char	*new_prompt;

	getcwd(data->prompt, sizeof(data->prompt));
	new_prompt = ft_strjoin(data->prompt, "$  ");
	data->new_prompt = new_prompt;
	input = readline(new_prompt);
	data->input = input;
}

/*
 * Objetivo: Función principal del programa minishell.
 * Inicializa las estructuras necesarias, gestiona la señalización,
 * muestra el prompt, lee la entrada del usuario, 
 * la procesa y ejecuta los comandos.
*/
static void	run_if_status_ok(t_tree *tree, t_data *data, int previous_status)
{
	if (!(g_status == 130 && previous_status != 130))
		runcmd(tree, data);
}

void	handle_input(t_data *data)
{
	int	previous_status;

	while (1)
	{
		draw_prompt(data);
		if (!data->input)
		{
			ft_free(data);
			break ;
		}
		if (check_input(data->input))
		{
			ft_free(data);
			continue ;
		}
		if (*data->input)
			add_history(data->input);
		previous_status = g_status;
		data->tree = parsepipe_tree(data->input);
		if (!data->tree)
			write(2, "token failed\n", 14);
		else
			run_if_status_ok(data->tree, data, previous_status);
		ft_free(data);
	}
}

/*
 * Objetivo: Punto de entrada del programa. Inicializa los datos,
 * configura las señales y llama a la función que maneja la entrada del usuario.
 */

int	main(void)
{
	t_data	*data;

	data = init_data();
	if (!data)
		panic("malloc failed\n", EXIT_FAILURE);
	sig_init();
	handle_input(data);
	rl_clear_history();
	free(data);
	return (g_status);
}
