/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 18:49:37 by ypacileo          #+#    #+#             */
/*   Updated: 2025/08/22 15:16:44 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_path_cmd(const char *dir, const char *cmd)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	return (full_path);
}

char	*check_direct_path(const char *cmd)
{
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

char	*search_in_paths(char **paths, int total_paths, const char *cmd)
{
	int		i;
	char	*full_path;

	i = 0;
	full_path = NULL;
	while (i < total_paths && paths[i])
	{
		full_path = join_path_cmd(paths[i], cmd);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		full_path = NULL;
		i++;
	}
	return (NULL);
}

char	*get_command_path(const char *cmd)
{
	char	*path_env;
	char	**paths;
	char	*result;
	int		total_paths;

	path_env = NULL;
	paths = NULL;
	result = NULL;
	total_paths = 0;
	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (check_direct_path(cmd));
	path_env = ft_getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	total_paths = count_split(paths);
	result = search_in_paths(paths, total_paths, cmd);
	free_split(&paths, total_paths);
	return (result);
}
