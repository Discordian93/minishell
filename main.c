#include "minishell.h"

// panic(): Imprime mensaje de error y sale
void panic(char *msg) 
{
    perror(msg);
    exit(1);
}

// Inicializa el árbol apuntando a NULL
t_tree_node *init_tree(void)
{
    return (NULL);
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
    int status;
    

    if (!tree)
        return;

    if (is_node_type(tree, "EXEC"))
    {
        t_exec *exec = (t_exec *)tree->objeto;
        printf("[EXEC] Ejecutando: %s\n", exec->argv[0]);

        pid = fork();
        if (pid == -1)
            panic("fork failed");

        if (pid == 0)
        {
            // Proceso hijo ejecuta el comando
            /*int i = 0;
            while (exec->argv[i])
            {
                printf("argv[%d] = %s\n", i, exec->argv[i]);
                i++;
            }*/
            execvp(exec->argv[0], exec->argv);
            panic("execvp failed");
        }
        else
        {
            // Proceso padre espera al hijo
            waitpid(pid, &status, 0);
        }
    }
    else if (is_node_type(tree, "REDIR"))
    {
        t_redir *redir = (t_redir *)tree->objeto;
        printf("[REDIR] Redirigiendo fd %d a: %s (mode: %d)\n",
               redir->fd, redir->file, redir->mode);

        /*guardamos una copia*/
        int saved_fd = dup(redir->fd);
        if (saved_fd < 0)
            panic("dup failed");

        int fd = open(redir->file, redir->mode, 0644);
        if (fd < 0)
            panic("open failed");
        
        /*ahora STDOUT_FILENO comperte el mismo recurso que fd*/
        if (dup2(fd, redir->fd) < 0)
            panic("dup2 failed");
        close(fd);

        runcmd_tree(tree->left);  // Ejecutar el nodo hijo

        /*ahora STDOUT_FILENO vuelve a conectarse con la terminal*/
        dup2(saved_fd, redir->fd); 
        close(saved_fd);
    }
}

// Ejemplo de uso en main
int main(void)
{
    char *input;
    t_tree_node *tree;


    while (1)
    {
        input = readline("minishell> ");

        if (!input || strcmp(input, "exit") == 0)
        {
            free(input);
            break;
        }

        if (*input)
            add_history(input);

        // Inicializar y parsear el árbol
        tree = init_tree();
        tree = parsepipe_tree(input);
        
        if (tree == NULL)
        {
            write(2, "Error\n", 7);
        }
        else
        {
            runcmd_tree(tree);
            //free_tree(tree);
        }

        free(input);
    }
    
    return (0);
}