/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultins.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:42:06 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/10 18:57:07 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin_parents(t_exec *exec)
{
	if (ft_strncmp(exec->argv[0],"cd",3) == 0)
		return 1;
	else if (ft_strncmp(exec->argv[0],"export", 7) == 0)
		return (1);
	else if (ft_strncmp(exec->argv[0],"unset", 6) == 0)
		return (1);
	else if (ft_strncmp(exec->argv[0], "exit", 5) == 0)
		return (1);
	else
		return (0);
}

int is_builtin_child(t_exec *exec)
{
	if (ft_strncmp(exec->argv[0],"echo", 5) == 0)
		return (1);
	else if(ft_strncmp(exec->argv[0], "pwd", 4)  == 0)
		return (1);
	else if (ft_strncmp(exec->argv[0], "env", 4) == 0)
		return (1);
	return (0);
}


		
void	ft_cd(char **exe)
{
	char *home;

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
	
int is_echo_n(char *arg)
{
    int i = 0;

    if (!arg || arg[0] != '-')
        return 0;
    i++;
    while (arg[i] == 'n')
        i++;
    return (arg[i] == '\0');
}


void ft_echo(char **exec)
{
    int i;
    int newline;
	
	i = 1;
	newline = 1;
    // Verificar múltiples -n seguidos
    while (exec[i] && is_echo_n(exec[i]))
    {
        newline = 0;
        i++;
    }

    // Imprimir los argumentos restantes
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
		panic("pwd\n");
	printf("%s\n", buf);
}

void ft_exit(char **args, t_data *data)
{
    int arg_count;
	
	
	arg_count = count_split(args);
    printf("exit\n");
    if (arg_count == 1)
    {
        free_and_exit(data, status);
    }
    else if (arg_count == 2)
    {
        if (!is_valid_number(args[1]))
        {
            write(2, "minishell: exit: ", 17);
            write(2, args[1], ft_strlen(args[1]));
            write(2, ": numeric argument required\n", 28);
            free_and_exit(data, 2);
        }
        status = ft_atoi(args[1]);
        status = ((status % 256) + 256) % 256;
        free_and_exit(data, status);
    }
    else
    {
        write(2, "minishell: exit: too many arguments\n", 37);
        status = 1;
        return;
    }
}

void execute_builtin_child(t_exec *exec)
{
	if (ft_strncmp(exec->argv[0], "echo", 5) == 0)
		ft_echo(exec->argv);
	else if (ft_strncmp(exec->argv[0], "pwd", 4) == 0)
		ft_pwd();
	else if (ft_strncmp(exec->argv[0], "env", 4) == 0)
		ft_env();
	exit(EXIT_SUCCESS);
	// ... otros builtins
}

void	execute_builtin_parents(t_exec *exec, t_data *data)
{
	if (ft_strncmp(exec->argv[0], "cd", 3) == 0)
		ft_cd(exec->argv);
	else if (ft_strncmp(exec->argv[0], "export", 7) == 0)
		export(exec->argv);
	else if (ft_strncmp(exec->argv[0],"unset", 6) == 0)
		ft_unset(exec->argv);
	else if (ft_strncmp(exec->argv[0], "exit", 5) == 0)
        ft_exit(exec->argv, data);
	return ;
}

/*
int main()
{
	char *str[] = {"echo","-n", "-n","hola","pedro", NULL };
	ft_echo(str);
}
*/