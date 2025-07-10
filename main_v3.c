#include "minishell.h"

// Estructura genérica de nodo del árbol (reemplaza a t_cmd)
typedef struct s_tree_node
{
    char *etiqueta;                  // Etiqueta que identifica el tipo: "EXEC", "REDIR", "PIPE"
    void *objeto;                    // Puntero genérico al objeto (exec, redir, pipe, etc.)
    struct s_tree_node *left;       // Nodo hijo izquierdo
    struct s_tree_node *right;      // Nodo hijo derecho
} t_tree_node;

// Estructuras específicas SIN campo type (ya no heredan de t_cmd)
typedef struct s_exec
{
    char *argv[MAXARGS];
} t_exec;

typedef struct s_redir 
{
    char *file;                      // Nombre del archivo destino o fuente
    int mode;                        // Modo de apertura (O_RDONLY, O_WRONLY, etc.)
    int fd;                          // File descriptor a redirigir (0 = stdin, 1 = stdout)
} t_redir;



// Funciones para manejo del árbol
t_tree_node *init_tree(void);
t_tree_node *create_tree_node(void *objeto, char *etiqueta);
void free_tree(t_tree_node *root);
void print_tree(t_tree_node *root, int depth);
int is_node_type(t_tree_node *node, char *tipo);


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

// Libera recursivamente todo el árbol
void free_tree(t_tree_node *root)
{
    if (!root)
        return;
    
    // Liberar recursivamente los subárboles
    free_tree(root->left);
    free_tree(root->right);
    
    // Liberar el objeto según su etiqueta
    if (root->objeto)
    {
        if (is_node_type(root, "EXEC"))
            free(root->objeto); // Liberar estructura exec
        else if (is_node_type(root, "REDIR"))
            free(root->objeto); // Liberar estructura redir
        else if (is_node_type(root, "PIPE"))
            free(root->objeto); // Liberar estructura pipe
    }
    
    // Liberar el nodo actual
    free(root);
}


// Función auxiliar para determinar el tipo de redirección
void get_redir_info(char *token, int *mode, int *fd)
{
    if (ft_strncmp(token, ">>", 2) == 0)
    {
        *mode = O_WRONLY | O_CREAT | O_APPEND;
        *fd = 1;
    }
    else if (ft_strncmp(token, ">", 1) == 0)
    {
        *mode = O_WRONLY | O_CREAT | O_TRUNC;
        *fd = 1;
    }
    else if (ft_strncmp(token, "<<", 2) == 0)
    {
        *mode = O_RDONLY; // Heredoc - necesita tratamiento especial
        *fd = 0;
    }
    else if (ft_strncmp(token, "<", 1) == 0)
    {
        *mode = O_RDONLY;
        *fd = 0;
    }
}


// Función auxiliar para verificar si un token es una redirección
int is_redirection(char *token)
{
    return (ft_strncmp(token, ">", 1) == 0 || 
            ft_strncmp(token, "<", 1) == 0 ||
            ft_strncmp(token, ">>", 2) == 0 ||
            ft_strncmp(token, "<<", 2) == 0);
}

// Parsea comandos simples con múltiples redirecciones encadenadas
t_tree_node *parseexec_tree(char *input)
{
    char **token;
    t_exec *exec;
    t_redir *redir;
    t_tree_node *exec_node;
    t_tree_node *current_node;
    t_tree_node *redir_node;
    int i, j, mode, fd;

    // Crear estructura exec
    exec = malloc(sizeof(t_exec));
    if (!exec)
        panic("malloc failed");
    
    // Inicializar argv
    j = 0;
    while(j < MAXARGS)
    {
        exec->argv[j] = NULL;
        j++;
    }

    token = ft_token(input);
    i = 0;
    j = 0;
    
    // Crear nodo EXEC base
    exec_node = create_tree_node((void *)exec, "EXEC");
    current_node = exec_node;
    
    while (token[i] != NULL)
    {
        if (is_redirection(token[i]))
        {
            i++; // Avanzar al nombre del archivo
            if (!token[i])
                panic("Falta archivo después de redirección");
            
            // Obtener información de la redirección
            get_redir_info(token[i - 1], &mode, &fd);
            
            // Crear estructura de redirección
            redir = malloc(sizeof(t_redir));
            if (!redir)
                panic("malloc failed");
            
            redir->file = token[i]; // Asignación directa - token[i] ya es una copia
            redir->mode = mode;
            redir->fd = fd;
            
            // Crear nodo de redirección
            redir_node = create_tree_node((void *)redir, "REDIR");
            
            // Encadenar: la redirección actual apunta al nodo anterior
            redir_node->left = current_node;
            
            // La redirección actual se convierte en el nodo actual
            current_node = redir_node;
        }
        else
        {
            // Es un argumento del comando
            exec->argv[j++] = token[i]; // Asignación directa - token[i] ya es una copia
        }
        i++;
    }
    
    // Liberar tokens originales
    //free_tokens(token);
    
    return (current_node);
}

// Parsea pipes y crea un nodo del árbol
t_tree_node *parsepipe_tree(char *input)
{
    char *pipe_pos;
    char *left_part;
    char *right_part;
    t_tree_node *left_node;
    t_tree_node *right_node;
    t_tree_node *pipe_node;
    char *pipe;

    pipe_pos = ft_strchr(input, '|');
    if (pipe_pos == NULL)
    {
        printf("[DEBUG] caso base: %s\n", input);
        return (parseexec_tree(input));
    }

    printf("[DEBUG] Dividiendo: %s\n", input);
    *pipe_pos = '\0';
    left_part = input;
    right_part = pipe_pos + 1;
    
    left_node = parseexec_tree(left_part);
    right_node = parsepipe_tree(right_part);
    
    // Crear estructura pipe (puede estar vacía)
    pipe = malloc(sizeof(char));
    if (!pipe)
        panic("malloc failed");
    
    // Crear nodo del árbol para el pipe
    pipe_node = create_tree_node((void *)pipe, "PIPE");
    pipe_node->left = left_node;
    pipe_node->right = right_node;
    
    printf("[DEBUG] Nodo pipe creado\n");
    return (pipe_node);
}


// Función auxiliar para imprimir el árbol (debugging)
void print_tree(t_tree_node *root, int depth)
{
    int i;

    if (!root)
        return;
    
    // Imprimir espacios para mostrar la profundidad
    i = 0;
    while (i < depth)
    {
        printf("  ");
        i++;
    }
    
    // Mostrar la etiqueta del nodo
    printf("[%s] ", root->etiqueta ? root->etiqueta : "NULL");
    
    // Mostrar información específica según el tipo
    if (is_node_type(root, "EXEC") && root->objeto)
    {
        t_exec *exec = (t_exec *)root->objeto;
        printf("comando: %s\n", exec->argv[0] ? exec->argv[0] : "NULL");
    }
    else if (is_node_type(root, "REDIR") && root->objeto)
    {
        t_redir *redir = (t_redir *)root->objeto;
        printf("archivo: %s (fd: %d)\n", redir->file ? redir->file : "NULL", redir->fd);
    }
    else if (is_node_type(root, "PIPE"))
    {
        printf("pipe\n");
    }
    else
    {
        printf("desconocido\n");
    }
    
    // Imprimir recursivamente los hijos
    if (root->left || root->right)
    {
        print_tree(root->left, depth + 1);
        print_tree(root->right, depth + 1);
    }
}

/*
// Ejemplo de función para ejecutar comandos usando el árbol
void runcmd_tree(t_tree_node *tree)
{
    if (!tree)
        return;
    
    if (is_node_type(tree, "EXEC"))
    {
        t_exec *exec = (t_exec *)tree->objeto;
        printf("[EXEC] Ejecutando: %s\n", exec->argv[0]);
        // Aquí iría la lógica de execvp, fork, etc.
    }
    else if (is_node_type(tree, "REDIR"))
    {
        t_redir *redir = (t_redir *)tree->objeto;
        printf("[REDIR] Redirigiendo a: %s\n", redir->file);
        // Aquí iría la lógica de open, dup2, etc.
        runcmd_tree(tree->left); // Ejecutar el comando hijo
    }
    else if (is_node_type(tree, "PIPE"))
    {
        printf("[PIPE] Creando pipe\n");
        // Aquí iría la lógica de pipe, fork, etc.
        runcmd_tree(tree->left);  // Ejecutar lado izquierdo
        runcmd_tree(tree->right); // Ejecutar lado derecho
    }
}
*/
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
            printf("[DEBUG] Árbol creado:\n");
            print_tree(tree, 0);
            
            // Ejecutar comandos
            //runcmd_tree(tree);
            
            // Liberar el árbol
            free_tree(tree);
        }

        free(input);
    }
    
    return (0);
}