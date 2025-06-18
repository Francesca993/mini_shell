/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 21:58:25 by francesca         #+#    #+#             */
/*   Updated: 2025/06/18 12:19:33 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

/*
Requisiti del subject
Comando: echo
Deve supportare l'opzione -n, che sopprime il newline finale.
Non sono richieste altre opzioni.
Il comportamento deve essere coerente con Bash.
Il comando echo in una shell serve per stampare una stringa di testo sullo 
standard output (tipicamente il terminale).
Per impostazione predefinita, echo aggiunge un ritorno a capo (\n) 
alla fine del testo stampato.
-> Opzione -n -> Serve a disattivare il newline finale.
*/

/*
Serve a validare casi come -nnnn e respipngere casi come -nnno
*/
static int check_valid_no_new_line(char *arg)
{
    int j;

    j = 2;
    while (arg[j] == 'n')
        j++;
    if (arg[j] != '\0')
        return (1);
    else
        return (0);
}

void ft_echo(char **args)
{
    int i;
    int newline;

    newline = 1;
    i = 1;
    // Gestione dell'opzione -n (può essere ripetuta: -n -n -n)
    while (args[i] && ft_strncmp(args[i], "-n", 2) == 0)
    {
        if ((newline = check_valid_no_new_line(args[i])) == 1)
            break;
        i++;
    }
    // Stampa tutti gli argomenti separati da spazio
    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
    g_exit_status = 0;
}
