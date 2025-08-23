/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 09:36:17 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/22 14:19:39 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_split(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] != NULL)
		i++;
	return (i);
}

void	free_split(char ***s, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		free((*s)[i]);
		i++;
	}
	free(*s);
	*s = NULL;
}

void	ft_free(t_data *data)
{
	if (!data)
		return ;
	if (data->input)
		free(data->input);
	if (data->new_prompt)
		free(data->new_prompt);
	if (data->tree)
		free_tree(data->tree);
	data->input = NULL;
	data->new_prompt = NULL;
	data->tree = NULL;
}

void	free_and_exit(t_data *data, int status)
{
	ft_free(data);
	rl_clear_history();
	if (data)
		free(data);
	exit(status);
}
