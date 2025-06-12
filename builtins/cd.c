/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 14:03:41 by francesca         #+#    #+#             */
/*   Updated: 2025/06/12 14:25:01 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

/*
Obiettivo: cd [path]
Solo un argomento (cd path)
Nessuna opzione richiesta
Deve modificare la working directory del processo
Deve gestire errori: path mancante, path non esistente, troppi argomenti
📦 Comportamenti richiesti
Comando	Azione
cd /tmp	Cambia directory a /tmp
cd	Bash va in $HOME (non richiesto dal subject)
cd pippo pluto	Errore: too many arguments
cd nonesiste	Errore: no such file or directory
*/
int ft_cd(char **args)
{
    if (!args[1])
    {
        exit_shell(1, "minishell: cd: missing argument\n");
        return (1);
    }

    if (args[2])
    {
        exit_shell(1, "minishell: cd: too many arguments\n");
        return (1);
    }

    if (chdir(args[1]) != 0)
    {
        exit_shell(1, "minishell: cd: no such file or directory\n");
        return (1);
    }
    g_exit_status = 0;
    return (1);
}
