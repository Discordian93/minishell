#include "minishell.h"

/* Auxiliary function to create a "KEY=VALUE" string */
char	*create_env_string(char *key, char *value)
{
	char	*str;
	
	str = malloc(1);
	if (!str)
		return (NULL);
	str[0] = '\0';
	append(&str, key);
	append(&str, "=");
	append(&str, value);
	return (str);
}

/* Auxiliary function to free an entire environment array */
void	free_environ(char **env)
{
	size_t	i;
	
	if (!env)
		return ;
	i = 0;
	while (env[i])
		free(env[i++]);
	free(env);
}

/* Auxiliary function to free partial environment (for error handling) */
void	free_partial_environ(char **env, size_t count)
{
	size_t	i;
	
	if (!env)
		return ;
	i = 0;
	while (i < count)
		free(env[i++]);
	free(env);
}

/* Auxiliary function to count environment variables */
size_t	count_environ(char **env)
{
	size_t	i;
	
	i = 0;
	while (env[i])
		i++;
	return (i);
}

char	**initialize_environ(char **env)
{
	size_t	i;
	char	**myenv;
	size_t	j;
	
	i = count_environ(env);
	myenv = malloc(sizeof(char*) * (i + 1));
	if (!myenv)
		return (NULL);
	j = 0;
	while (j < i)
	{
		myenv[j] = ft_strndup(env[j], ft_strlen(env[j]));
		if (!myenv[j])
		{
			free_partial_environ(myenv, j);
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
	i = count_environ(env);
	myenv = malloc(sizeof(char*) * (i + 2));
	if (!myenv)
		exit(1);
	j = 0;
	while (j < i)
	{
		myenv[j] = ft_strndup(env[j], ft_strlen(env[j]));
		if (!myenv[j])
		{
			free_partial_environ(myenv, j);
			return ;
		}
		j++;
	}
	myenv[j] = create_env_string(key, value);
	if (!myenv[j])
	{
		free_partial_environ(myenv, j);
		return ;
	}
	myenv[j + 1] = NULL;
	free_environ(env);
	*envdir = myenv;
}

char	*my_getenv(char *key, char **env)
{
	size_t	i;
	size_t	j;
	char	*split;
	
	i = count_environ(env);
	j = 0;
	while (j < i)
	{
		split = ft_strchr(env[j], '=');
		if (split)
		{
			*split = '\0';
			if (ft_strncmp(key, env[j], ft_strlen(key)) == 0 && 
			    ft_strlen(env[j]) == ft_strlen(key))
			{	
				*split = '=';
				return (split + 1);
			}
			*split = '=';
		}
		j++;
	}
	return (NULL);
}

void	my_setenv(char *key, char *value, char ***env)
{
	size_t	i;
	size_t	j;
	char	*split;
	char	**environ;
	char	*new_str;
	
	environ = *env;
	i = count_environ(environ);
	j = 0;
	while (j < i)
	{
		split = ft_strchr(environ[j], '=');
		if (split)
		{
			*split = '\0';
			if (ft_strncmp(key, environ[j], ft_strlen(key)) == 0 && 
			    ft_strlen(environ[j]) == ft_strlen(key))
			{
				new_str = create_env_string(key, value);
				if (!new_str)
					exit(1);
				free(environ[j]);
				environ[j] = new_str;
				return ;
			}
			else
				*split = '=';
		}
		j++;
	}
	add_to_environ(env, key, value);
	return ;
}

/* Auxiliary function to find environment variable index */
int	find_env_index(char *key, char **env)
{
	size_t	i;
	char	*split;
	
	i = 0;
	while (env[i])
	{
		split = ft_strchr(env[i], '=');
		if (split)
		{
			*split = '\0';
			if (ft_strncmp(key, env[i], ft_strlen(key)) == 0 && 
			    ft_strlen(env[i]) == ft_strlen(key))
			{
				*split = '=';
				return (i);
			}
			*split = '=';
		}
		i++;
	}
	return (-1);
}

/* Auxiliary function to copy environment excluding one index */
char	**copy_environ_except(char **env, size_t skip_index)
{
	size_t	i;
	size_t	j;
	size_t	count;
	char	**new_env;
	
	count = count_environ(env);
	new_env = malloc(sizeof(char*) * count);
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
	while (env[i])
	{
		if (i != skip_index)
		{
			new_env[j] = ft_strndup(env[i], ft_strlen(env[i]));
			if (!new_env[j])
				return (free_partial_environ(new_env, j), NULL);
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	return (new_env);
}

void	my_unsetenv(char *key, char ***env)
{
	int		index;
	char	**new_env;
	
	index = find_env_index(key, *env);
	if (index == -1)
		return ;
	new_env = copy_environ_except(*env, index);
	if (!new_env)
		exit(1);
	free_environ(*env);
	*env = new_env;
}

/*
	op 1 is getenv
	op 2 is setenv
	op 3 is getting entire env
	op 4 is unsetenv
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
		my_setenv(key, value, &my_environ);
		return (NULL);
	}
	else if (op == 3)
		return ((char **) my_environ);
	else if (op == 4)
	{
		my_unsetenv(key, &my_environ);
		return (NULL);
	}
	else
		return (NULL);
}

/*void	*handle_environ(char *key, size_t op)
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
}*/

char	*ft_getenv(char *key)
{	
	return ((char *) handle_environ(key, NULL, 1));
}

void	ft_setenv(char *key, char *value)
{	
	handle_environ(key, value, 2);
	return ;
}

char	**env(void)
{
	return ((char **) handle_environ(NULL, NULL, 3));
}

void	ft_unsetenv(char *key)
{	
	handle_environ(key, NULL, 4);
	return ;
}

void export_one(char *s)
{
	char	*split;
	if (!s)
		return ;
	split = ft_strchr(s, '=');
	if (!split)
		return ;
	*split = '\0';
	ft_setenv(s, split + 1);
	*split = '=';
}

void	export(char **s)
{
	s++;
	while (*s)
	{
		export_one(*s);
		s++;
	}
}