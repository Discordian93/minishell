/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_parents.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 22:17:19 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/20 22:46:00 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin_parents(t_exec *exec)
{
	if (ft_strncmp(exec->argv[0], "cd", 3) == 0)
		return (1);
	else if (ft_strncmp(exec->argv[0], "export", 7) == 0)
		return (1);
	else if (ft_strncmp(exec->argv[0], "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(exec->argv[0], "exit", 5) == 0)
		return (1);
	else
		return (0);
}

void	ft_cd(char **exe)
{
	char	*home;

	if (ft_strncmp(exe[0], "cd", 3) == 0 && count_split(exe) == 1)
	{
		home = getenv("HOME");
		if (!home)
			write(2, "cd: HOME not set\n", 18);
		else if (chdir(home) != 0)
			perror("cd");
	}
	else if (ft_strncmp(exe[0], "cd", 3) == 0 && count_split(exe) == 2)
	{
		if (chdir(exe[1]) != 0)
			perror("cd");
	}
	else
		write(2, "cd: usage: cd <dir>\n", 21);
}

void	ft_exit(char **args, t_data *data)
{
	int	arg_count;

	arg_count = count_split(args);
	printf("exit\n");
	if (arg_count == 1)
		free_and_exit(data, g_status);
	else if (arg_count == 2)
	{
		if (!is_valid_number(args[1]))
		{
			write(2, "minishell: exit: ", 17);
			write(2, args[1], ft_strlen(args[1]));
			write(2, ": numeric argument required\n", 28);
			free_and_exit(data, 2);
		}
		g_status = ft_atoi(args[1]);
		g_status = ((g_status % 256) + 256) % 256;
		free_and_exit(data, g_status);
	}
	else
	{
		write(2, "minishell: exit: too many arguments\n", 37);
		g_status = 1;
		return ;
	}
}
