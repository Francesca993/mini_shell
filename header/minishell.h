/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:38:15 by francesca         #+#    #+#             */
/*   Updated: 2025/06/12 09:53:15 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"         // Include le funzioni della libft
#include <stdio.h>       // printf, perror, strerror – I/O standard e gestione errori
#include <stdlib.h>      // malloc, free, exit, getenv – Allocazione, uscita, variabili ambiente
#include <unistd.h>      // write, read, close, access, fork, execve, pipe, dup, dup2,
// isatty, ttyname, ttyslot, getcwd, chdir, unlink – Operazioni su file/processi/terminali
#include <fcntl.h>       // open – Apertura file
#include <sys/wait.h>    // wait, waitpid, wait3, wait4 – Gestione processi figli
#include <signal.h>      // signal, sigaction, sigemptyset, sigaddset, kill – Gestione segnali
#include <sys/stat.h>    // stat, lstat, fstat – Informazioni su file
#include <dirent.h>      // opendir, readdir, closedir – Accesso alle directory
#include <string.h>      // strerror – Descrizione stringa errore
#include <termios.h>     // tcsetattr, tcgetattr – Configurazione terminale
#include <termcap.h>     // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs – Gestione capacità del terminale
#include <readline/readline.h> // readline – Lettura interattiva da terminale
#include <readline/history.h>  // add_history, rl_clear_history, rl_on_new_line,
// rl_replace_line, rl_redisplay – Cronologia e manipolazione linea input

#include "envp.h"
#include "parser.h"
#include "execution.h"
#include "builtins.h"

//extern int g_exit_status;

#endif