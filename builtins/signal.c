/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 09:50:36 by francesca         #+#    #+#             */
/*   Updated: 2025/06/12 10:26:20 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

/*
I segnali sono un meccanismo del sistema operativo per notificare ai processi 
che si è verificato un evento, come ad esempio:
SIGINT (Ctrl-C): interruzione -> Ctrl-C	SIGINT	Interruzione	Mostrare una nuova riga con prompt
SIGQUIT (Ctrl-): quit (interrompe e core dump) -> Ctrl-\	SIGQUIT	Quit (dump del core)	Non deve fare nulla
SIGTERM: terminazione -> Ctrl-D	(EOF)	Fine input (non è un segnale)	Uscire dalla shell
*/

/*
SIGINT → Ctrl-C
Ricevuto quando l’utente preme Ctrl-C
Comportamento richiesto:
Mostrare un newline (\n)
Mostrare di nuovo il prompt
Non uscire dalla shell
*/
void handle_sigint(int signo)
{
    g_exit_status = 130; // 128 + SIGINT
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

/*
SIGQUIT → Ctrl-\
Ricevuto con Ctrl-\
Comportamento richiesto:
Non fare nulla
Ignorarlo
*/
signal(SIGQUIT, SIG_IGN); // ignora completamente il segnale

/*
Ctrl-D → EOF (fine input)
Non è un segnale, ma readline restituisce NULL
Comportamento richiesto:
Uscire dalla shell pulitamente
*/

