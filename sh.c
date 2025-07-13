/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 18:49:37 by ypacileo          #+#    #+#             */
/*   Updated: 2025/07/13 19:25:08 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

/**
 * Concatena dos cadenas en una nueva cadena
 * @param s1: primera cadena (se libera automáticamente)
 * @param s2: segunda cadena a concatenar
 * @return: nueva cadena concatenada o NULL en caso de error
 */
char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*result;
	size_t	i;
	size_t	j;

	if (!s2)
		return (NULL);
	if (!s1)
	{
		s1 = malloc(1);
		if (!s1)
			return (NULL);
		s1[0] = '\0';
	}
	result = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!result)
	{
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

int count_split(char **str)
{
	int i = 0;
	while(str[i] != NULL)
		i++;
	return (i);
}

// Libera la memoria ya asignada si ocurre un error en medio del proceso
void free_split(char ***s, int index)
{
    int i = 0;
    while (i < index)
    {
        free((*s)[i]);
        i++;
    }
    free(*s);
    *s = NULL;
}

// Función auxiliar que busca la ruta absoluta del comando que se quiere ejecutar
// por ejemplo, si cmd = "ls", busca en cada carpeta de $PATH si existe un binario "ls"
char *get_command_path(const char *cmd)
{
    // getenv("PATH") devuelve el valor de la variable de entorno PATH (una lista de rutas separadas por ':')
    char *path_env = getenv("PATH");

    // Divide el string PATH en partes, separadas por ':' (como ["/bin", "/usr/bin", ...])
    char **paths = ft_split(path_env, ':');
    char *full_path = NULL;
    int i = 0;

    while (paths && paths[i])
    {
        // Une cada directorio con el nombre del comando, por ejemplo: "/bin/ls"
        full_path = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin_free(full_path, cmd); // Une y libera el string anterior

        // Verifica si el archivo existe y es ejecutable
        if (access(full_path, X_OK) == 0)
        {
            free_split(&paths, count_split(paths));
            return full_path; // Devuelve el primer path que sea válido
        }

        free(full_path); // Libera si no es válido
        i++;
    }
    free_split(&paths, count_split(paths)); // Libera el array de paths
    return NULL; // Si no se encontró un ejecutable, se devuelve NULL
}

int main()
{
	char *cmd = get_command_path("cat");
	printf("%s\n", cmd);
	free(cmd);
	return (0);
}