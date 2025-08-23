/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 22:20:53 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/19 22:48:45 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	should_apply_redirs_only(t_exec *exec, t_tree *tree)
{
	if (!exec || !exec->argv[0])
	{
		apply_redirs_only(tree->left);
		return (1);
	}
	return (0);
}

int	should_execute_builtin_parent(t_exec *exec)
{
	if (is_builtin_parents(exec) && exec)
		return (1);
	return (0);
}

void	child_process(t_tree *tree, t_data *data, t_exec *exec)
{
	sig_default();
	runcmd(tree->left, data);
	if (is_builtin_child(exec) && exec)
		execute_builtin_child(exec);
	else
		execute_external_command(exec);
}

void	parent_process(pid_t pid, int *st)
{
	sig_ignore();
	waitpid(pid, st, 0);
	g_status = decode_wait_status(*st);
	sig_init();
}

void	run_exec(t_tree *tree, t_data *data)
{
	t_exec	*exec;
	pid_t	pid;
	int		st;

	exec = (t_exec *)tree->obj;
	if (should_apply_redirs_only(exec, tree))
		return ;
	if (should_execute_builtin_parent(exec))
	{
		execute_builtin_parents(exec, data);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		return ;
	}
	if (pid == 0)
		child_process(tree, data, exec);
	else
		parent_process(pid, &st);
}
