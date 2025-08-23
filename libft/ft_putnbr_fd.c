/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 12:04:12 by ypacileo          #+#    #+#             */
/*   Updated: 2024/07/04 15:08:24 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include <fcntl.h>  // Para open
#include <unistd.h> // Para write y close
#include <stdio.h>  // Para perror*/
#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		if (n == -2147483648)
		{
			ft_putchar_fd('2', fd);
			n = -147483648;
		}
		ft_putnbr_fd(n * -1, fd);
	}
	else if (n < 10)
		ft_putchar_fd((char)n + 48, fd);
	else
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putchar_fd(n % 10 + 48, fd);
	}
}

/*  int main()
{
int fd = open("archivonum.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("Error al abrir el archivo");
        return 1;
    }

    // Escribir una cadena en el archivo
    ft_putnbr_fd(-2147483648, fd);

    if (close(fd) == -1)
    {
        perror("Error al cerrar el archivo");
        return 1;
    }

    printf("Escritura en el archivo exitosa.\n");
    return 0;
}*/