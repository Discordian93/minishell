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

void	add_to_environ(char ***envdir, char *key, char *value)
{
	size_t	i;
	char	**myenv;
	char	**env;
	size_t	j;

	env = *envdir;
	i = 0;
	while (env[i])
		i++;
	myenv = malloc(sizeof(char*) * (i + 2));
	if (!myenv)
		exit(1) ;
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
			return ;
		}
		j++;
	}
	myenv[j] = malloc(1);
	if (!(myenv[j]))
		return ;
	myenv[j][0] = '\0';
	append(&(myenv[j]), key);
	append(&(myenv[j]), "=");
	append(&(myenv[j]), value);
	myenv[j+1] = NULL;
	i = 0;
	while (i < j)
		free(env[i++]);
	free(env);
	*envdir = myenv;
}

char	*my_getenv(char *key, char **env)
{
	size_t	i;
	size_t	j;
	char	*split;

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
				return(split + 1);
			}
			*split = '=';
		}
		j++;
	}
	return (NULL);
}

void	my_setenv(char *key, char *value, char **env)
{
	size_t	i;
	size_t	j;
	char	*split;

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
				free(env[j]);
				env[j] = malloc(1);
				if (!env[j])
					exit (1);
				env[j][0] = '\0';
				append(&(env[j]), key);
				append(&(env[j]), "=");
				append(&(env[j]), value);
				return ;
			}
			else
				*split = '=';
		}
		j++;
	}
	add_to_environ(&env, key, value);
	return ;
}

/*
	op 1 is getenv
	op 2 is setenv
	op 3 is getting entire env
*/
void	*handle_environ(char *key, char *value, size_t op)
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
	else if (op == 2)
	{
		my_setenv(key, value, my_environ);
		return (NULL);
	}
	else if (op == 3)
		return ((char **) my_environ);
	else
		return (NULL);
}

/* void	*handle_environ(char *key, size_t op)
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
		return ((char *) my_getenv(key, my_environ));
	else if (op == 3)
		return ((char **) my_environ);
	else
		return (NULL);
} */

char	*ft_getenv(char *key)
{	
	return ((char *) handle_environ(key, NULL, 1));
}

char	**env(void)
{
	return ((char **) handle_environ(NULL, NULL, 3));
}