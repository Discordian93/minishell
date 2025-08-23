/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 15:49:36 by ypacileo          #+#    #+#             */
/*   Updated: 2024/05/26 16:19:12 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*cdst;
	unsigned const char	*csrc;
	size_t				i;

	if (!dst && !src)
		return (0);
	i = 0;
	cdst = (unsigned char *) dst;
	csrc = (const unsigned char *) src;
	while (i < n)
	{
		cdst[i] = csrc[i];
		i++;
	}
	return (dst);
}
