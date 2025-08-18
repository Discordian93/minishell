/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 13:07:56 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/18 22:08:30 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int status = 0;


int decode_wait_status(int st)
{
    if (WIFEXITED(st))
        return WEXITSTATUS(st);
    if (WIFSIGNALED(st)) 
    {
        int sig = WTERMSIG(st);
        // Imprimir mensaje para señales comunes como bash
        if (sig == SIGINT)
            write(STDERR_FILENO, "\n", 1);
        else if (sig == SIGQUIT)
            write(STDERR_FILENO, "Quit (core dumped)\n", 19);
        return (128 + sig);
    }
    return (0);
}




/**
 * Manejador de señales para el proceso padre (shell interactiva).
 * Maneja SIGINT (Ctrl+C) cuando el shell está esperando input.
 */
void	sig_handler(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;

	if (sig == SIGINT)
	{
        // Actualizar status cuando Ctrl+C se presiona en el prompt
        status = 130; // 128 + SIGINT(2)
		write(STDOUT_FILENO, "\n", 1);       
		rl_on_new_line();                    
		rl_replace_line("", 0);             
		rl_redisplay();                     
	}
}

/**
 * Inicializa el manejo de señales personalizado para la shell padre.
 * Se usa SA_SIGINFO para que el manejador reciba contexto adicional.
 */


void	sig_init(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART | SA_SIGINFO;
	sa.sa_sigaction = &sig_handler;
	sigaction(SIGINT, &sa, NULL);     // Ctrl+C

	// Para SIGQUIT, asegúrate de ignorarla por completo
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);

}


/**
 * Ignora temporalmente señales.
 * Útil en el padre mientras espera hijos (evita doble manejo).
 */
void	sig_ignore(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

/**
 * Restaura el comportamiento por defecto del sistema.
 * Útil en los procesos hijos, antes de ejecutar comandos externos.
 */
void	sig_default(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}



/**
 * Alternativa simple para restaurar señales usando signal().
 * Hace lo mismo que sig_default() pero más directo.
 */
void	sig_reset(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}



/*static void set_handler(int signo, void (*handler)(int), int flags)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;          // puede ser una función, SIG_IGN o SIG_DFL
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = flags;              // p.ej. 0 o SA_RESTART
    if (sigaction(signo, &sa, NULL) == -1)
        perror("sigaction");
}
*/
/*void setup_signals_parent(void)
{
    set_handler(SIGINT,  sigint_parent, 0);                  // Ctrl-C
    set_handler(SIGQUIT, (void (*)(int))SIG_IGN, 0);         
}
*/