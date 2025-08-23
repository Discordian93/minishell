/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 19:14:40 by ypacileo          #+#    #+#             */
/*   Updated: 2024/06/30 13:59:04 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include <fcntl.h>  // Para open
#include <unistd.h> // Para write y close
#include <stdio.h>*/
#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	if (fd >= 0)
		write (fd, &c, sizeof(char));
}

/*int main()
{
    int fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("Error al abrir el archivo");
        return 1;
    }

    // Escribir algunos caracteres en el archivo
    ft_putchar_fd('H', fd);
    ft_putchar_fd('o', fd);
    ft_putchar_fd('l', fd);
    ft_putchar_fd('a', fd);
    ft_putchar_fd('\n', fd);

    if (close(fd) == -1)
    {
        perror("Error al cerrar el archivo");
        return 1;
    }

    return 0;
}*/
