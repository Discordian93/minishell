/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 21:55:01 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/22 16:29:44 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_token(char *input)
{
    char    **tok = ft_token(input);
    int n;
    int i;

    if (!tok)
        return (0);
    n = count_split(tok);
    i = 0;
    while (i < n)
    {
        if (EQ(tok[i], "|"))
        {
            if(ft_strlen(tok[i]) != 1)
            {
                status = 2;
                free_split(&tok,count_split(tok));
                return (0);	
            }
            if (i == 0 || i == n - 1 || EQ(tok[i+1], "|"))
            {
                status = 2;
                free_split(&tok, n);
                return (0);
            }
        }
        else if (EQ(tok[i], "<") || EQ(tok[i], ">") \
			|| EQ(tok[i], ">>") || EQ(tok[i], "<<"))
        {
            if(ft_strlen(tok[i]) > 2)
            {
                status = 2;
                free_split(&tok,count_split(tok));
                return (0);	
            }
            if (i == n - 1)
            {
                status = 2;
                free_split(&tok, n);
                return (0);
            }
            if (IS_OP(tok[i+1]))
            {
                status = 2;
                free_split(&tok, n);
                return (0);
            }
        }
        i++;
    }
    free_split(&tok, n);
    return (1);
}
