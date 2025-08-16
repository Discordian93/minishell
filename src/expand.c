/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 16:47:30 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/16 18:41:34 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append(char **s1, char *s2)
{
	char	*appended;

	appended = ft_strjoin(*s1, s2);
	if (appended)
	{
		free(*s1);
		*s1 = appended;
	}
	else
		exit(1);
}

void expand_sig(char **result)
{
    char *sig;

    sig = ft_itoa(status);
    if (!sig)
        return ;
    append(result, sig);
    free(sig);
}

void expand_var(char **token, char **result)
{
    char    *varname;
    char    *varval;
    char c[2];
    
    (*token)++;
    if (**token == '?')
    {
        (*token)++;
        expand_sig(result);
        return ;
    }
    varname = malloc(1);
    if (!varname)
        exit(1);
    *varname = '\0';
    //printf("DEBUG: Building variable name...\n");
    while (ft_isalnum(**token) || **token == '_')
    {
        c[0] = **token;
        c[1] = '\0';
        append(&varname, c);
        (*token)++;
    }
    //printf("DEBUG: Variable name: '%s'\n", varname);
    varval = ft_getenv(varname);
    //printf("DEBUG: Variable value: '%s'\n", varval ? varval : "(null)");
    if (varval)
        append(result, varval);
    free(varname);
}

void expand_quoted(char **token, char **result)
{
    char    quote;
    char    c[2];

    quote = **token;
    (*token)++;
    while (**token != quote)
    {
        if (quote == '"' && **token == '$')
            expand_var(token, result);
        else
        {
            c[0] = **token;
            c[1] = '\0';
            append(result, c);
            (*token)++;
            //printf("avanza...\n");
        }
    }
    (*token)++;
}


char *expand_token(char *token)
{
    char    *result;
    char    c[2];
    
    result = malloc(1);
    if (!result)
        return (NULL);
    *result = '\0';
    //printf("DEBUG: expand_token starting with: '%s'\n", token);
    while(*token)
    {
        //printf("DEBUG: Current char: '%c'\n", *token);
        if (*token != '\'' && *token != '"' && *token != '$')
        {
            c[0] = *token;
            c[1] = '\0';
            append(&result, c);
            token++;
        }
        else if (*token == '\'' || *token == '"')
            expand_quoted(&token, &result);
        else if (*token == '$')
            expand_var(&token, &result);
    }
    //printf("DEBUG: expand_token result: '%s'\n", result);
    return (result);
}

char    **expand_vars(char **tokens)
{   
    char    **expanded;
    int     i;
    int     j;

    i = 0;
    while (tokens && tokens[i])
        i++;
    expanded = malloc(sizeof(char*) * (i +1));
    if (!expanded)
        return (NULL);
    j = 0;
    while (j < i)
    {
        expanded[j] = expand_token(tokens[j]);
        j++;
    }
    expanded[j] = NULL;
    return (expanded);
}



char	*ft_strndup(char *str, size_t n)
{
	char	*dup;
	size_t	len;
	size_t	i;

    i= 0;
	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (n < len)
		len = n;
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	while (i < len)
	{
        dup[i] = str[i];
        i++;
    }
	dup[i] = '\0';
	return (dup);
}


/*
// Función de prueba para expand_vars
int main(void)
{
    // Tokens simulados manualmente (como si fueran de ft_token)
    char *tokens[] = {
        "echo",
        "$USER",
        "'$HOME'",
        "\"$HOME/Documents\"",
        "Texto plano",
        "q\"$SHELL path\"",
        NULL
    };

    char **expanded = expand_vars(tokens);
    if (!expanded)
    {
        fprintf(stderr, "Error: expand_vars falló\n");
        return 1;
    }

    printf("\nTokens expandidos:\n");
    for (int i = 0; expanded[i]; i++)
    {
        printf("[%d]: %s\n", i, expanded[i]);
        free(expanded[i]);
    }
    free(expanded);
    return 0;
}
*/