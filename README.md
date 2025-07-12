Resumen de Bash para los requisitos de Minishell·

1. Mostrar una entrada (prompt) y tener un historial funcional:
    ◦ Prompt: Bash utiliza las variables PS1 y PS2 para definir las cadenas del prompt
	. PS1 es el prompt primario que se muestra antes de leer el primer comando, y PS2 el secundario para comandos multilínea. Estas variables se expanden antes de ser mostradas. Además, la variable de vector PROMPT_COMMANDS puede contener comandos a ejecutar antes de mostrar el prompt principal
.
    ◦ Historial: Bash proporciona acceso al historial de comandos si la opción -o history está habilitada
	. El número de comandos a guardar en la lista del historial se controla con la variable HISTSIZE (por defecto 500). El historial se inicializa desde el archivo especificado por HISTFILE (por defecto ~/.bash_history) al inicio del intérprete, y las últimas $HISTSIZE líneas se copian a este archivo cuando el intérprete se cierra. La opción histappend (configurada con shopt) determina si las líneas se añaden o sobrescriben el archivo del historial. Las variables HISTIGNORE y HISTCONTROL también afectan cómo se guardan los comandos en el historial. Los comandos history y fc permiten manipular la lista del historial y el archivo
.
2. Buscar y ejecutar el ejecutable correcto (basado en PATH o rutas relativas/absolutas):

    ◦ Cuando Bash ejecuta un comando simple, primero intenta localizarlo
	. Si el nombre del comando no contiene barras (/), busca una función del intérprete con ese nombre, luego un comando integrado (builtin), y finalmente busca un archivo ejecutable con ese nombre en cada elemento de la variable $PATH
.
    ◦ Si el nombre del comando contiene una o más barras, Bash ejecuta el programa nombrado directamente en un entorno de ejecución aislado
	. La variable PATH es una lista de directorios separados por dos puntos donde el intérprete busca comandos
.
3. Gestionar señales (ctrl-C, ctrl-D, ctrl-\):

    ◦ ctrl-C (SIGINT): Cuando Bash es interactivo, SIGINT es atrapada y manejada
	. Interrumpirá algunos comandos integrados del intérprete. Si Bash está esperando que un comando se complete y recibe una señal para la cual se ha establecido una trap, la trap se ejecutará después de que el comando se complete. El proyecto especifica que ctrl-C imprime una nueva entrada en una línea nueva
.
    ◦ ctrl-D (EOF): Controla la acción del intérprete al recibir un carácter EOF como única entrada

	. Si la variable IGNOREEOF está asignada, su valor denota el número de caracteres EOF consecutivos que pueden ser leídos antes de que el intérprete finalice. Si la variable no existe, EOF significa el final de la entrada. El proyecto especifica que ctrl-D termina el shell
.
    ◦ ctrl-\ (SIGQUIT): Bash ignora SIGQUIT en todos los casos
. 	El proyecto especifica que ctrl-\ no hace nada
.
    ◦ Variable Global para Señales: El requisito de "evitar utilizar más de una variable global para indicar la recepción de una señal" y que "esta variable global no puede proporcionar ninguna otra información o datos que el número de una señal recibida"
 	es una restricción de diseño específica de tu proyecto Minishell, no una descripción del comportamiento interno de Bash. Bash define el comportamiento de las señales (trap) y cómo los procesos hijos heredan los manejadores de señal
.
4. Gestionar comillas (' y "):

    ◦ Comillas simples ('): Encerrar caracteres en comillas simples preserva el valor literal de cada carácter dentro de las comillas. Una comilla simple no puede encontrarse entre comillas simples
.
    ◦ Comillas dobles ("): Encerrar caracteres en comillas dobles preserva el valor literal de todos los caracteres entre ellas, con la excepción de $, \`` (sustitución de comandos), ` (escape de barra invertida) y ! (expansión de historial, si está activada)
.
5. Implementar redirecciones (<, >, <<, >>):

    ◦ Redirección de entrada (<): Abre el archivo especificado para lectura en el descriptor de archivo n (o entrada estándar, descriptor 0, si n se omite)
.
    ◦ Redirección de salida (>): Abre el archivo especificado para escritura en el descriptor de archivo n (o salida estándar, descriptor 1, si n se omite). Crea el archivo si no existe; si existe, lo trunca a tamaño cero
	.La opción noclobber del comando set puede evitar la sobrescritura, pero >| la fuerza
.
    ◦ Here-document (<<): Instruye al intérprete a leer la entrada de la fuente actual hasta que se encuentre una línea que contenga solo el delimitador. Todas las líneas leídas hasta ese punto se utilizan como entrada estándar para un comando
	. Si se usa <<-, se eliminan todos los caracteres de tabulación iniciales de las líneas de entrada y de la línea del delimitador

    ◦ Redirección de salida en modo append (>>): Hace que la salida se añada al final del archivo especificado. Si el archivo no existe, se crea
.
6. Implementar pipes (|):
    ◦ Una tubería (pipe) es una secuencia de uno o más comandos separados por el operador de control |
	. El propósito es conectar la salida estándar del comando anterior a la entrada estándar del comando siguiente. Bash espera a que todos los comandos en la tubería se completen antes de devolver un valor

7. Gestionar variables de entorno ($):

    ◦ Bash realiza expansión de parámetros y variables
	. Una variable es una entidad que almacena valores y puede ser un nombre, un número o uno de los caracteres especiales. Bash escanea su propio entorno al inicio y crea un parámetro para cada nombre encontrado, marcándolo automáticamente para exportar a los procesos hijos
.
8. Gestionar \$? (estado de salida):
   
    ◦ El parámetro especial $? se expande al estado de salida de la tubería en primer plano ejecutada más recientemente
	. El estado de salida de un comando ejecutado es un valor entre 0 y 255. Por convención, cero indica éxito, y un valor distinto de cero indica fallo. Si un comando finaliza con una señal fatal cuyo número es N, Bash usa el valor 128+N como estado de salida
.
10. Implementar los built-ins:

    ◦ echo -n: El comando echo imprime sus argumentos en la salida estándar
	. La opción -n suprime la nueva línea final
.
    ◦ cd: Cambia el directorio actual de trabajo
	. El manual de Bash indica que puede cambiar a rutas relativas o absolutas. Minishell especifica que solo con una ruta relativa o absoluta
.
    ◦ pwd: Imprime el directorio de trabajo actual
	. Minishell especifica sin opciones. Bash tiene opciones -L (lógico) y -P (físico) para pwd, pero no son parte del requisito de Minishell.
    ◦ export: Marca variables para exportación automática al entorno de los procesos hijos Minishell especifica sin opciones
.
    ◦ unset: Elimina variables o funciones del intérprete
. 	Minishell especifica sin opciones
.
    ◦ env: Este comando, aunque no se detalla su funcionalidad específica en el extracto de Bash proporcionado, se menciona como un comando integrado en el glosario
.	 Minishell especifica sin opciones o argumentos
.
    ◦ exit: Causa que el shell finalice
	. Puede tomar un argumento entero como estado de salida. Minishell especifica sin opciones
.
11. Parte extra (&&, || con paréntesis para prioridades):

    ◦ Listas de comandos (&&, ||): En Bash, && (lista AND) y || (lista OR) son operadores de control que tienen la misma precedencia
	. instrucción1 && instrucción2 significa que instrucción2 se ejecuta si y solo si instrucción1 tiene un estado de salida de cero (éxito). instrucción1 || instrucción2 significa que instrucción2 se ejecuta si y solo si instrucción1 tiene un estado de salida distinto de cero (fallo)
.
    ◦ Agrupación con paréntesis (()): Ubicar una lista de instrucciones entre paréntesis () hace que se cree un entorno de subintérprete, y cada una de las instrucciones en la lista se ejecuta en ese subintérprete
	. Esto permite agrupar comandos y controlar sus redirecciones como una unidad. Dado que se ejecuta en un subintérprete, las asignaciones de variables no permanecen en vigor después de que el subintérprete finaliza
.
