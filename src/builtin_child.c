/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 22:17:58 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/20 22:37:07 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin_child(t_exec *exec)
{
	if (ft_strncmp(exec->argv[0], "echo", 5) == 0)
		return (1);
	else if (ft_strncmp(exec->argv[0], "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(exec->argv[0], "env", 4) == 0)
		return (1);
	return (0);
}

int	is_echo_n(char *arg)
{
	int	i;

	i = 0;
	if (!arg || arg[0] != '-')
		return (0);
	i++;
	while (arg[i] == 'n')
		i++;
	return (arg[i] == '\0');
}

void	ft_echo(char **exec)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (exec[i] && is_echo_n(exec[i]))
	{
		newline = 0;
		i++;
	}
	while (exec[i])
	{
		printf("%s", exec[i]);
		if (exec[i + 1])
			printf(" ");
		i++;
	}
	if (newline == 1)
		printf("\n");
}

void	ft_pwd(void)
{
	char	buf[PATH_MAX];

	if (getcwd(buf, sizeof(buf)) == NULL)
		panic("pwd\n", 127);
	printf("%s\n", buf);
}
