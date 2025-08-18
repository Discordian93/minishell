/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 20:45:42 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/18 22:09:47 by yuliano          ###   ########.fr       */
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
    ft_memset(data->prompt, 0, sizeof(data->prompt));
	return (data);

}

int main(void)
{
    t_data  *data;
   
    data = init_data();
    if (!data)
        panic("malloc failed\n");
    sig_init();
    while (1)
    {
        getcwd(data->prompt, sizeof(data->prompt));
        data->new_prompt = ft_strjoin(data->prompt, "$  ");
        data->input = readline(data->new_prompt);
        if (!data->input)
        {
            ft_free(data);
            break;
        }
        if (check_input(data->input))
		{
			ft_free(data);
			 continue;
		}
        if (*data->input)
            add_history(data->input);
        data->tree = parsepipe_tree(data->input);
        if (!data->tree)
            write(2, "token failed\n", 14);
        else
            runcmd(data->tree, data);
        ft_free(data);
	}
	rl_clear_history(); 
    free(data);
    return (status);
} 
