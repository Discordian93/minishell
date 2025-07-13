/* utils.c - Utility functions */
#include "minishell.h"

char *ft_strdup(const char *s)
{
    size_t len;
    char *dup;

    if (!s)
        return NULL;
    len = strlen(s);
    dup = malloc(len + 1);
    if (!dup)
        return NULL;
    strcpy(dup, s);
    return dup;
}

char *ft_strndup(const char *s, size_t n)
{
    size_t len;
    char *dup;

    if (!s)
        return NULL;
    len = strlen(s);
    if (n < len)
        len = n;
    dup = malloc(len + 1);
    if (!dup)
        return NULL;
    strncpy(dup, s, len);
    dup[len] = '\0';
    return dup;
}

void *ft_calloc(size_t count, size_t size)
{
    void *ptr;
    size_t total;

    total = count * size;
    ptr = malloc(total);
    if (!ptr)
        return NULL;
    memset(ptr, 0, total);
    return ptr;
}