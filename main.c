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


/**
 * Ejecuta un árbol de comandos de forma recursiva
 * Maneja diferentes tipos de nodos: EXEC (ejecución) y REDIR (redirección)
 * @param tree: nodo del árbol a ejecutar
 */
/*
void runcmd_tree(t_tree_node *tree)
{
    pid_t pid;
    int status;
    
    if (!tree)
        return;
    
    if (is_node_type(tree, "EXEC"))
    {
        t_exec *exec = (t_exec *)tree->objeto;
        //printf("[EXEC] Ejecutando: %s\n", exec->argv[0]);
        
        // Creamos un nuevo proceso hijo
        pid = fork();
        if (pid == -1)
            panic("fork failed");
        
        if (pid == 0) // Proceso hijo
        {
            // Obtenemos el path absoluto del binario
            char *path = get_command_path(exec->argv[0]);
            if (!path)
                panic("command not found");
            
            // Usamos execve, que reemplaza el proceso actual por el binario especificado
            // execve(path_binario, argumentos, variables_de_entorno)
            execve(path, exec->argv, NULL);
            
            // ✅ Liberar memoria antes de panic (solo se ejecuta si execve falla)
            free(path);
            panic("execve failed");
        }
        else // Proceso padre
        {
            // Espera a que termine el proceso hijo
            waitpid(pid, &status, 0);
        }
    }
    else if (is_node_type(tree, "REDIR"))
    {
        t_redir *redir = (t_redir *)tree->objeto;
        printf("[REDIR] Redirigiendo fd %d a: %s (mode: %d)\n",
               redir->fd, redir->file, redir->mode);
        
        // Guardamos el descriptor original para restaurarlo luego
        int copy_fd = dup(redir->fd);
        if (copy_fd < 0)
            panic("dup failed");
        
        // Abrimos el archivo para redirigir
        int fd = open(redir->file, redir->mode, 0644);
        if (fd < 0) 
        {
            close(copy_fd);
            panic("open failed");
        }
        
        // Redirigimos el fd original al archivo
        if (dup2(fd, redir->fd) < 0) 
        {
            close(fd);
            close(copy_fd);
            panic("dup2 failed");
        }
        
        // Cerramos el descriptor del archivo (ya está duplicado)
        close(fd);
        
        // Ejecutamos el subárbol redirigido
        runcmd_tree(tree->left);
        
        // Restauramos el descriptor original
        if (dup2(copy_fd, redir->fd) < 0)
            panic("dup2 restore failed");
        close(copy_fd);
    }
    
    else if (is_node_type(tree, "PIPE"))
    {
        // TODO: Implementar manejo de pipes
        printf("[PIPE] Tipo de nodo no implementado aún\n");
    }
    
    
}
*/

void runcmd_tree(t_tree_node *tree)
{
    pid_t pid;
    int status;

    if (!tree)
        return;

    if (is_node_type(tree, "EXEC"))
    {
        t_exec *exec = (t_exec *)tree->objeto;
        pid = fork();
        if (pid == -1)
            panic("fork failed");

        if (pid == 0)
        {
            // Proceso hijo aplica redirecciones antes de execve
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
    else if (is_node_type(tree, "REDIR"))
    {
        t_redir *redir = (t_redir *)tree->objeto;
        int fd = open(redir->file, redir->mode, 0644);
        if (fd < 0)
            panic("open failed");

        if (dup2(fd, redir->fd) < 0)
        {
            close(fd);
            panic("dup2 failed");
        }
        close(fd);
        runcmd_tree(tree->left);
    }
}

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