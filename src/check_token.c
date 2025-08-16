/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 21:55:01 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/09 23:42:14 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_token(char *input)
{
    char **tok = ft_token(input);
    int n, i;

    if (!tok)
        return 0;
    n = count_split(tok);

    i = 0;
    while (i < n)
    {
        if (EQ(tok[i], "|"))
        {
			if(ft_strlen(tok[i]) != 1)
			{
				free_split(&tok,count_split(tok));
				return (0);	
			}
            // no al inicio ni al final, ni pipes consecutivos
            if (i == 0 || i == n - 1 || EQ(tok[i+1], "|"))
            {
                free_split(&tok, n);
                return 0;
            }
        }
        else if (EQ(tok[i], "<") || EQ(tok[i], ">") \
			|| EQ(tok[i], ">>") || EQ(tok[i], "<<"))
        {
			if(ft_strlen(tok[i]) > 2)
			{
				free_split(&tok,count_split(tok));
				return (0);	
			}
            // requieren operando a la derecha
            if (i == n - 1)
            {
                free_split(&tok, n);
                return 0;
            }

            // el siguiente NO puede ser otro operador (|, <, >, <<, >>)
            if (IS_OP(tok[i+1]))
            {
                free_split(&tok, n);
                return 0;
            }

            // comparaciones exactas (ya cubiertas por EQ)
            // no hace falta chequear strlen; EQ ya descarta tokens mal formados como ">>>"
        }
        
        i++;
    }

    free_split(&tok, n);
    return 1;
}
