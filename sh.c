/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 18:49:37 by ypacileo          #+#    #+#             */
/*   Updated: 2025/07/17 23:01:49 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

/**
 * Concatena dos cadenas en una nueva cadena
 * @param s1: primera cadena (se libera automáticamente)
 * @param s2: segunda cadena a concatenar
 * @return: nueva cadena concatenada o NULL en caso de error
 */
char *ft_strjoin_free(char *s1, const char *s2)
{
    char *result;
    size_t i;
    size_t j;
    
    if (!s2)
		return (NULL);
    
    if (!s1) {
        s1 = malloc(1);
        if (!s1)
            return (NULL);
        s1[0] = '\0';
    }
    
    result = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
    if (!result) {
        free(s1);
        return (NULL);
    }
    
    i = 0;
    while (s1[i]) 
	{
        result[i] = s1[i];
        i++;
    }
    
    j = 0;
    while (s2[j])
        result[i++] = s2[j++];
    
    result[i] = '\0';
    free(s1);
    return (result);
}

/**
 * Cuenta el número de elementos en un array de strings
 * @param str: array de strings terminado en NULL
 * @return: número de elementos
 */
int count_split(char **str)
{
    int i = 0;
    
    if (!str)
        return (0);
    
    while (str[i] != NULL)
        i++;
    
    return (i);
}

/**
 * Libera la memoria de un array de strings
 * @param s: puntero al array de strings
 * @param total_count: número total de elementos a liberar
 */

/*
void free_split(char ***s, int total_count)
{
    int i = 0;
    
    if (!s || !*s)
        return;
    
    while (i < total_count && (*s)[i]) {
        free((*s)[i]);
        (*s)[i] = NULL;
        i++;
    }
    
    free(*s);
    *s = NULL;
}

*/

/**
 * Función auxiliar que busca la ruta absoluta del comando que se quiere ejecutar
 * Comportamiento similar a bash:
 * - Si cmd contiene '/', lo trata como ruta absoluta/relativa
 * - Si no, busca en cada directorio del PATH
 * @param cmd: nombre del comando a buscar
 * @return: ruta completa del ejecutable o NULL si no se encuentra
 */
char *get_command_path(const char *cmd)
{
    char *path_env;
    char **paths;
    char *full_path;
    char *temp;
    int total_paths;
    int i;
    
    if (!cmd || !*cmd)
        return (NULL);
    
    // Si el comando contiene '/', tratarlo como ruta absoluta/relativa
    if (ft_strchr(cmd, '/')) 
	{
        if (access(cmd, X_OK) == 0)
            return (ft_strdup(cmd));
        return (NULL);
    }
    
    // Obtener la variable de entorno PATH
    path_env = getenv("PATH");
    if (!path_env)
        return (NULL);
    
    // Dividir PATH en directorios
    paths = ft_split(path_env, ':');
    if (!paths)
        return (NULL);
    
    total_paths = count_split(paths);
    full_path = NULL;
    
    // Buscar en cada directorio del PATH
    i = 0;
    while (i < total_paths && paths[i]) 
	{
        // Crear la ruta completa: directorio + "/" + comando
        temp = ft_strjoin(paths[i], "/");
        if (!temp) 
		{
            free_split(&paths, total_paths);
            return (NULL);
        }
        
        full_path = ft_strjoin_free(temp, cmd);
        if (!full_path) {
            free_split(&paths, total_paths);
            return (NULL);
        }
        
        // Verificar si el archivo existe y es ejecutable
        if (access(full_path, X_OK) == 0) {
            free_split(&paths, total_paths);
            return (full_path);  // Éxito: devolver la ruta encontrada
        }
        
        // No encontrado en este directorio, liberar y continuar
        free(full_path);
        full_path = NULL;
        i++;
    }
    
    // Limpiar memoria y devolver NULL si no se encontró
    free_split(&paths, total_paths);
    return (NULL);
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