/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 22:33:01 by yuliano           #+#    #+#             */
/*   Updated: 2025/07/26 23:34:24 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_SHELL
#define MINI_SHELL

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
# include "libft/libft.h"


# define MAXARGS 15
# define READ_END 0
# define WRITE_END 1



typedef enum e_redir_type
{
    FILE_TRUNC,
    FILE_APPEND,
    FILE_IN,
    HEREDOC
} t_redir_type;

// Estructura genérica de nodo del árbol
typedef struct s_tree
{
    char *label;
    void *obj;
    struct s_tree *left;
    struct s_tree *right;
} t_tree;

typedef struct s_data
{
    char prompt[1024];
    char *input;
    char *new_prompt;
    t_tree *tree;
}   t_data;


typedef struct s_exec
{
    char *argv[MAXARGS];
} t_exec;

typedef struct s_redir 
{
    t_redir_type   type;
    char *file;
    int mode;
    int fd;                          
} t_redir;



int     skip_separators(const char *s, int i);
int     skip_quoted_word(const char *s, int i);
int     skip_unquoted_word(const char *s, int i);
int     count_words(const char *s);
t_tree	*create_tree_node(void *obj, char *label);
void	free_tree(t_tree *root);
int		is_node_type(t_tree	*node, char *type);
int		is_redirection(char *token);
void	get_redir_info(char *token, int *mode, int *fd);
void    ft_redir(t_redir **redir,char *file, int mode, int fd);
t_tree	*parseexec_tree(char *input);
char	**ft_token(const char *str);
t_tree	*parsepipe_tree(char *input);
void	panic(char *msg);
char	*ft_strjoin_free(char *s1, const char *s2);
int		count_split(char **str);
void	free_split(char ***s, int total_count);
char	*get_command_path(const char *cmd);
void	ft_cd(char **exe);
int		is_bultin(t_exec *exec);
void	free_tree(t_tree *node);
void	ft_free(t_data *data);
void	free_and_exit(t_data *data, int status);
void	run_exec(t_tree *tree, t_data *data);
void	execute_external_command(t_tree *tree, t_data *data, t_exec *exec);
void	run_redir(t_tree *tree, t_data *data);
void	run_pipe_child_left(t_tree *tree, t_data *data, int fd[2]);
void	run_pipe_child_right(t_tree *tree, t_data *data, int fd[2]);
void	run_pipe(t_tree *tree, t_data *data);
void	runcmd(t_tree *tree, t_data *data);
#endif