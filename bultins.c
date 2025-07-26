/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultins.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:42:06 by yuliano           #+#    #+#             */
/*   Updated: 2025/07/26 09:25:13 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_bultin(t_exec *exec)
{
	if (ft_strncmp(exec->argv[0],"cd",2) == 0)
		return (1);
	else
		return (0);
}

void	ft_cd(char **exe)
{

	if (ft_strncmp(exe[0], "cd", 2) == 0 && count_split(exe) == 1)
	{
		if(chdir(getenv("HOME")) != 0)
			perror("cd");
	}
	else if (ft_strncmp(exe[0],"cd", 2) == 0 && count_split(exe) == 2)
	{
		if (chdir(exe[1]) != 0)
			perror("cd");
	}
	else
		write(2, "cd: usage: cd <dir>\n", 21);
}

