/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 19:23:03 by ypacileo          #+#    #+#             */
/*   Updated: 2024/06/30 13:04:35 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include <fcntl.h>  // Para open
#include <unistd.h> // Para write y close
#include <stdio.h>  // Para perror*/
#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	size_t	i;

	i = ft_strlen(s);
	write(fd, s, sizeof(char) * i);
}

/*int main()
{
int fd = open("un_archivo.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("Error al abrir el archivo");
        return 1;
    }

    // Escribir una cadena en el archivo
    ft_putstr_fd("estoy escribiendo en un archivo.\n", fd);

    if (close(fd) == -1)
    {
        perror("Error al cerrar el archivo");
        return 1;
    }

    printf("Escritura en el archivo exitosa.\n");
    return 0;
}*/