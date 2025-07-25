#include "minishell.h"

// panic(): Imprime mensaje de error y sale
void panic(char *msg) 
{
    perror(msg);
    exit(1);
}


// Crea un nuevo nodo del árbol con el objeto y etiqueta dados
t_tree_node *create_tree_node(void *objeto, char *etiqueta)
{
    t_tree_node *new_node;

    new_node = malloc(sizeof(t_tree_node));
    if (!new_node)
        panic("malloc failed in create_tree_node");
    
    new_node->etiqueta = etiqueta;
    new_node->objeto = objeto;
    new_node->left = NULL;
    new_node->right = NULL;
    
    return (new_node);
}

// Verifica si un nodo es de un tipo específico
int is_node_type(t_tree_node *node, char *tipo)
{
    if (!node || !node->etiqueta || !tipo)
        return (0);
    
    return (ft_strncmp(node->etiqueta, tipo, ft_strlen(tipo)) == 0);
}

void runcmd_tree(t_tree_node *tree)
{
    pid_t pid;
    pid_t pid_left;
    pid_t pid_right;
    int status;
    int fd[2];

    if (!tree)
        return;

    if (is_node_type(tree, "EXEC"))
    {
        t_exec *exec = (t_exec *)tree->objeto;
		if (ft_strncmp(exec->argv[0],"cd",2) == 0)
			ft_cd(exec->argv);
        else 
		{
			pid = fork();
        	if (pid == -1)
            panic("fork failed");

        	if (pid == 0)
        	{
            	runcmd_tree(tree->left);

            	char *path = get_command_path(exec->argv[0]);
				if (!path)
                	panic("command not found");
				execve(path, exec->argv, NULL);
            	free(path);
            	panic("execve failed");
        	}
        	else
        	{
            	waitpid(pid, &status, 0);
        	}
		}
    }
    else if (is_node_type(tree, "REDIR"))
    {
        t_redir *redir = (t_redir *)tree->objeto;
        int fd = open(redir->file, redir->mode, 0644);
        if (fd < 0)
            panic("open failed");
        if (dup2(fd, redir->fd) < 0)
    		panic("dup2 failed");
        close(fd);
        runcmd_tree(tree->left);
    }
    else if (is_node_type(tree, "PIPE"))
    {
        if (pipe(fd) == -1)
            panic("pipe failed");

        pid_left = fork();
        if (pid_left == -1)
            panic("fork failed");

        if (pid_left == 0)
        {
            close(fd[READ_END]);
            if (dup2(fd[WRITE_END], STDOUT_FILENO) < 0)
                panic("dup2 write failed");
            close(fd[WRITE_END]);
            runcmd_tree(tree->left);
            exit(EXIT_SUCCESS);
        }

        pid_right = fork();
        if (pid_right == -1)
            panic("fork failed");

        if (pid_right == 0)
        {
            close(fd[WRITE_END]);
            if (dup2(fd[READ_END], STDIN_FILENO) < 0)
                panic("dup2 read failed");
            close(fd[WRITE_END]);
            runcmd_tree(tree->right);
            exit(EXIT_SUCCESS);
        }

        close(fd[READ_END]);
        close(fd[WRITE_END]);
        waitpid(pid_left, &status, 0);
        waitpid(pid_right, &status, 0);
    }
}


int main(void)
{
    char *input;
	char prompt[1024];

    t_tree_node *tree;
	
    while (1)
    {
		getcwd(prompt, sizeof(prompt));
		char *new_prompt = ft_strjoin(prompt, "$ ");
        input = readline(new_prompt);

        if (!input || strcmp(input, "exit") == 0)
        {
            free(input);
            break;
        }

        if (*input)
            add_history(input);

        tree = parsepipe_tree(input);

        if (!tree)
            write(2, "Error\n", 6);
        else
            runcmd_tree(tree);

        free(input);
        rl_on_new_line();
    }
    return 0;
}



/*
void print_tree(t_tree_node *node, int level)
{
    if (!node)
        return;

    for (int i = 0; i < level; i++)
        printf("  ");

    if (is_node_type(node, "EXEC"))
    {
        t_exec *exec = (t_exec *)node->objeto;
        printf("[EXEC] %s\n", exec->argv[0]);
    }
    else if (is_node_type(node, "REDIR"))
    {
        t_redir *redir = (t_redir *)node->objeto;
        printf("[REDIR] fd=%d file=%s\n", redir->fd, redir->file);
    }

    print_tree(node->left, level + 1);
}

int main(void)
{
    char *input = "echo hola > file1 >> file2 < file3";
    printf("%s\n\n", input);
    t_tree_node *tree = parseexec_tree(input);

    printf("\n\033[1;32mÁrbol de ejecución generado:\033[0m\n");
    print_tree(tree, 0);

    // Aquí podrías liberar el árbol si tienes free_tree implementado
    return 0;
}
*/