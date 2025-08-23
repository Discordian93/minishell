/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 15:41:34 by ypacileo          #+#    #+#             */
/*   Updated: 2024/06/30 20:14:38 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdio.h>
#include "libft.h"

void	*ft_calloc(size_t num, size_t size)
{
	void	*ptr;

	ptr = malloc(num * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, num * size);
	return (ptr);
}

/*
int main() {
    // Número de elementos en el arreglo
    size_t numElements = 10;

    // Asignar memoria para el arreglo de caracteres usando calloc
    char *arr = (char *)ft_calloc(numElements, sizeof(char));

    // Verificar si la asignación de memoria fue exitosa
    if (arr == NULL) {
        printf("La asignación de memoria ha fallado.\n");
        return 1;
    }
	// Imprimir el arreglo para verificar que todos 
	los elementos están inicializados a cero
    printf("Arreglo inicializado a cero:\n");
    for (size_t i = 0; i < numElements; i++) {
        printf("%d ", arr[i]);  // Debería imprimir 0 para todos los elementos
    }
    printf("\n");

    // Liberar la memoria asignada
    free(arr);

    return 0;
}
*/