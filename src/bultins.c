/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultins.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:42:06 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/20 22:48:17 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_builtin_child(t_exec *exec)
{
	if (ft_strncmp(exec->argv[0], "echo", 5) == 0)
		ft_echo(exec->argv);
	else if (ft_strncmp(exec->argv[0], "pwd", 4) == 0)
		ft_pwd();
	else if (ft_strncmp(exec->argv[0], "env", 4) == 0)
		ft_env();
	exit(EXIT_SUCCESS);
}

void	execute_builtin_parents(t_exec *exec, t_data *data)
{
	if (ft_strncmp(exec->argv[0], "cd", 3) == 0)
		ft_cd(exec->argv);
	else if (ft_strncmp(exec->argv[0], "export", 7) == 0)
		export(exec->argv);
	else if (ft_strncmp(exec->argv[0], "unset", 6) == 0)
		ft_unset(exec->argv);
	else if (ft_strncmp(exec->argv[0], "exit", 5) == 0)
		ft_exit(exec->argv, data);
	return ;
}
