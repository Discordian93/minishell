/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 13:26:07 by ypacileo          #+#    #+#             */
/*   Updated: 2024/06/01 12:48:17 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset( void *str, int x, size_t leng)
{
	unsigned char	*s;
	size_t			i;	

	i = 0;
	s = str;
	while (i < leng)
	{
		s[i] = (unsigned char)x;
		i++;
	}
	return (s);
}
