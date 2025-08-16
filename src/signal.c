/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypacileo <ypacileo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 13:07:56 by yuliano           #+#    #+#             */
/*   Updated: 2025/08/16 14:24:08 by ypacileo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//int status = 0;
volatile sig_atomic_t status = 0;

void	sigint_parent(int signo)
{
    (void)signo;
    status = 130;            // 128 + SIGINT
      // como bash
	//printf("%d\n", status);
	//if (signo == SIGINT || signo == SIGQUIT )
	//{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();        // Notifica que estamos en nueva línea
		rl_replace_line("", 0);  // Reemplaza con cadena vacía
    	rl_redisplay();          // Actualiza la pantalla
    	
	//}
    	
}

void	reboot_prompt()
{
    // Limpiar la línea actual y refrescar la pantalla
	rl_on_new_line();        // Notifica que estamos en nueva línea
    rl_replace_line("", 0);  // Reemplaza con cadena vacía
    rl_redisplay();          // Actualiza la pantalla
    
}

int decode_wait_status(int st)
{
    if (WIFEXITED(st))
        return WEXITSTATUS(st);
    if (WIFSIGNALED(st)) 
        return (128 + WTERMSIG(st));
    return (0);
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