/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:49:55 by francesca         #+#    #+#             */
/*   Updated: 2025/06/30 08:48:39 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

/*
*Se l'utente chiude la shell con exit senza argomenti oppure preme 
*ctrl-D, il codice di uscita deve essere: 0.
*Se l'utente scrive: exit 42
*allora la shell deve terminare restituendo 42 come codice di uscita al sistema.
*Se l'utente scrive un comando qualsiasi che fallisce,
	il codice di uscita dovrebbe essere:
*0 → se il comando ha successo
*1 o altri → se fallisce, a seconda dell'errore
*Il valore deve essere salvato per $?,
	ma anche essere usato se la shell termina in seguito a quell'errore.
*/

static int	ft_is_numeric(const char *str)
{
	if (!str || *str == '\0')
		return (0);
	if (*str == '+' || *str == '-')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

/*
//  Gestisce:
// exit → esce con 0
// exit 42 → esce con 42
// exit pippo → errore + esce con 2
// exit 1 2 → errore + NON esce
*/
int	ft_exit(char **args)
{
	long	code;

	// write(2, "exit\n", 5);
	if (args[1])
	{
		if (args[2])
		{
			// non esce, ma segnala errore
			fprintf(stderr, "exit: too many arguments\n");
			return (1);
		}
		//Se al secondo argomentoo c'è un numero esce stampandolo
		if (!ft_is_numeric(args[1]))
		{
			fprintf(stderr, "exit: %s: numeric argument required\n",
					args[1]);
			exit_shell(2, "");
			return (0);
		}
		// Calcolo modulo 256 per restituire solo gli 8 bit
		code = ft_atoi(args[1]);
		write(2, "exit\n", 5);
		exit_shell(code % 256, NULL);
		return (0); // Esce
	}
	// Nessun argomento: esce con ultimo codice (tipicamente passato a parte)
	else
	{
		write(2, "exit\n", 5);
		exit_shell(0, NULL); // oppure `exit_shell(last_exit_code, NULL);`
	}
	return (0);
}
