/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 18:49:37 by ypacileo          #+#    #+#             */
/*   Updated: 2025/08/16 18:44:58 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Función auxiliar que busca la ruta absoluta del comando que se quiere ejecutar
 * Comportamiento similar a bash:
 * - Si cmd contiene '/', lo trata como ruta absoluta/relativa
 * - Si no, busca en cada directorio del PATH
 * @param cmd: nombre del comando a buscar
 * @return: ruta completa del ejecutable o NULL si no se encuentra
 */
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

/**
 * Verifica si la ruta dada del comando es directamente accesible y ejecutable.
 *
 * @param cmd La ruta del comando a verificar.
 * @return Una nueva cadena duplicada con la ruta del comando si es ejecutable,
 *         o NULL si la ruta no es accesible o no es ejecutable.
 *
 * Esta función utiliza la llamada al sistema access() para verificar si el archivo en la ruta
 * especificada existe y tiene permisos de ejecución. Si es así, retorna un duplicado de la ruta;
 * de lo contrario, retorna NULL.
 */
char	*check_direct_path(const char *cmd)
{
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

/**
 * search_in_paths - Busca el ejecutable de un comando en una lista de rutas.
 *
 * Objetivo:
 *   Recorre cada directorio en el array 'paths', une el nombre del comando 'cmd'
 *   a cada ruta, y verifica si el archivo resultante existe y es ejecutable.
 *   Si encuentra el ejecutable, retorna la ruta completa; si no, retorna NULL.
 *
 * @param paths        Array de cadenas con los directorios del PATH.
 * @param total_paths  Número total de directorios en el array paths.
 * @param cmd          Nombre del comando a buscar.
 * @return             Ruta completa del ejecutable si se encuentra y es ejecutable,
 *                     o NULL si no se encuentra en ninguno de los directorios.
 */
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

/**
 * get_command_path - Busca la ruta absoluta de un comando.
 * Objetivo: Dado el nombre de un comando, retorna la ruta absoluta 
 * 				del ejecutable
 *           si existe y es ejecutable, buscando en el PATH si es necesario.
 *           Si el comando contiene '/', se trata como ruta absoluta/relativa.
 *           Si no, busca en cada directorio del PATH.
 *           La función está dividida en funciones auxiliares 
 * 			para mayor claridad.
 * @param cmd: nombre del comando a buscar
 * @return: ruta completa del ejecutable o NULL si no se encuentra
 */

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

/*
int main()
{
	char *cmd = get_command_path("echo");
	if (cmd)
	{
		printf("%s\n", cmd);
		free(cmd);
	}
		
	else
		printf("NULL\n");
	
	return (0);
}*/