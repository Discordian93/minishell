# include "minishell.h"


char	**initialize_environ(char **env)
{
	size_t	i;
	char	**myenv;
	size_t	j;

	i = 0;
	while (env[i])
		i++;
	myenv = malloc(sizeof(char*) * (i + 1));
	if (!myenv)
		return (NULL);
	j = 0;
	while (j < i)
	{
		myenv[j] = ft_strndup(env[j], ft_strlen(env[j]));
		if (!myenv[j])
		{	
			i = 0;
			while (i < j)
				free(myenv[i++]);
			free(myenv);
			return (NULL);
		}
		j++;
	}
	myenv[j] = NULL;
	return (myenv);
}

char **my_getenv(char *key, char **env)
{
	size_t	i;
	size_t	j;
	char	*split;
	char	**val;

	val = malloc(sizeof(char*));
	if (!val)
		return (NULL);
	i = 0;
	while (env[i])
		i++;
	j = 0;
	while (j < i)
	{
		split = ft_strchr(env[j], '=');
		if (split)
		{
			*split = '\0';
			if (ft_strncmp(key, env[j], ft_strlen(key)) == 0 && ft_strlen(env[j]) == ft_strlen(key))
			{	
				*split = '=';
				*val = (split + 1);
				return (val);
			}
			*split = '=';
		}
		j++;
	}
	return (NULL);
}

/*
	op 1 is getenv
	op 2 is setenv
	op 3 is getting entire env
*/
/* char	*handle_environ(char *key, char *value, size_t op)
{
	static size_t	initialized = 0;
	static char**	my_environ = NULL;
	extern char**	environ;

	if (initialized == 0)
	{
		my_environ = initialize_environ(environ);
		if (!my_environ)
			return (NULL);
		initialized = 1;
	}
	if (op == 1)
		return (my_getenv(key, my_environ));
	else
		return (NULL);
} */

char	**handle_environ(char *key, size_t op)
{
	static size_t	initialized = 0;
	static char**	my_environ = NULL;
	extern char**	environ;

	if (initialized == 0)
	{
		my_environ = initialize_environ(environ);
		if (!my_environ)
			return (NULL);
		initialized = 1;
	}
	if (op == 1)
		return (my_getenv(key, my_environ));
	else if (op == 3)
		return (my_environ);
	else
		return (NULL);
}

char	*ft_getenv(char *key)
{	
	char	**val;
	char	*result;

	 val = handle_environ(key,  1);
	 if (val)
	 	result = *val;
	else
		result = NULL;
	free(val);
	return (result);
}

char	**env(void)
{
	return (handle_environ(NULL, 3));
}