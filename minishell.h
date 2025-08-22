/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 22:33:01 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/22 18:03:25 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <stdlib.h>
# include <signal.h>
# include "libft/libft.h"

# define PATH_MAX 4096
# define MAXARGS 15
# define READ_END 0
# define WRITE_END 1
# define MODE_HEREDOC 0x8000


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
    char prompt[PATH_MAX];
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
    char *file;
    int mode;
    int fd;                          
} t_redir;

extern int	status;
int		count_words(const char *s);
t_tree *	create_tree_node(void *obj, char *label);
void		free_tree(t_tree *root);
int		is_node_type(t_tree	*node, char *type);
int		is_redirection(char *token);
void		get_redir_info(char *token, int *mode, int *fd);
void		ft_redir(t_redir **redir,char *file, int mode, int fd);
t_exec *	initialize_exec(void);
t_tree *	build_exec_tree(t_exec *exec, char **token);
void		handle_redirection(t_tree **current, char **token, int *i);
t_tree *	parseexec_tree(char *input);
char **		ft_token(const char *str);
t_tree *	parsepipe_tree(char *input);
void		panic(char *msg, int status);
char *		ft_strjoin_free(char *s1, const char *s2);
int		count_split(char **str);
void		free_split(char ***s, int total_count);
char *		get_command_path(const char *cmd);
void		ft_echo(char **exec);
void		ft_cd(char **exe);
void		ft_pwd();
int		is_builtin_parents(t_exec *exec);
int		is_builtin_child(t_exec *exec);
void		free_tree(t_tree *node);
void		ft_free(t_data *data);
void		free_and_exit(t_data *data, int status);
void		run_exec(t_tree *tree, t_data *data);
void		execute_external_command(t_exec *exec);
void		execute_builtin_child(t_exec *exec);
void		run_redir(t_tree *tree, t_data *data);
void		run_pipe_child_left(t_tree *tree, t_data *data, int fd[2]);
void		run_pipe_child_right(t_tree *tree, t_data *data, int fd[2]);
void		run_pipe(t_tree *tree, t_data *data);
void		runcmd(t_tree *tree, t_data *data);
char **		expand_vars(char **tokens);
char *		expand_token(char *token);
char **		env(void);
char *		ft_getenv(char *key);
char *		ft_strndup(char *str, size_t n);
int		check_token(char *input);
void		append(char **s1, char *s2);
void		expand_var(char **token, char **result);
int		handle_heredoc(char *delim);
int		is_in_quotation(char *deli);
void		ft_export(char **s);
void		add_to_environ(char ***envdir, char *key, char *value);
void		execute_builtin_parents(t_exec *exec, t_data *data);
void		ft_env(void);
void		ft_unsetenv(char *key);
void		ft_unset(char **args);
const char *	skip_spaces(const char *s);
int		skip_quoted(const char *s, int i);
int		token_size(const char *str);
char **		init_token_array(const char *str, int *len);
void		copy_token(char *dst, const char *src, int size);
void		sigint_parent(int signo);
void		setup_signals_parent(void);
int		decode_wait_status(int st);
void		reboot_prompt();
void		setup_signals_parent(void);
void		sig_handler(int sig, siginfo_t *info, void *context);
void		sig_init(void);
void		sig_ignore(void);
void		sig_default(void);
int		is_valid_number(char *str);
int		check_input(char *str);
char *		my_ft_strchr(const char *s, int c);
void		apply_redirs_only(t_tree *node);
void		ft_exit(char **args, t_data *data);
char *		ft_getenv(char *key);
void		ft_setenv(char *key, char *value);
char **		env(void);
void		ft_unsetenv(char *key);
void		export_one(char *s);
void		export(char **s);
void		ft_env(void);
void		ft_unset(char **args);
char *		create_env_string(char *key, char *value);
void		free_environ(char **env);
void		free_partial_environ(char **env, size_t count);
size_t		count_environ(char **env);
char **		initialize_environ(char **env);
void		add_to_environ(char ***envdir, char *key, char *value);
char *		my_getenv(char *key, char **env);
void		my_setenv(char *key, char *value, char ***env);
int		find_env_index(char *key, char **env);
char **		copy_environ_except(char **env, size_t skip_index);
void		my_unsetenv(char *key, char ***env);
void *		handle_environ(char *key, char *value, size_t op);
void		expand_sig(char **result);
char *		get_variable_name(char **token);
int		check_operator(const char *s, int i);
void		init_heredoc_pipe(int pipefd[2]);
int		is_delimiter(const char *line, const char *cmp_delim, size_t len);
void		heredoc_loop(int write_fd, const char *cmp_delim, int quoted_delim);
void		write_line_to_pipe(int fd, const char *line);
char *		strip_wrapping_quotes(const char *s);
void		append_char(char **res, char c);
char *		expand_heredoc_line(char *line);
void		process_heredoc_line(int fd, char *line, int quoted_delim);
int		is_wrapped_in_quotes(const char *s);
void		process_normal_redir(t_redir **redir, char *token, int mode, int fd);
void		process_heredoc_redir(t_redir **redir, char *delim, int fd);
char *		fd_to_string(int fd);
#endif
