/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 15:49:59 by francesca         #+#    #+#             */
/*   Updated: 2025/06/27 16:04:28 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

/*
** =========================
** FUNZIONE: unset_variable_2
** -------------------------
** Rimuove una singola variabile d’ambiente dal my_env.
**
** Parametri:
** - my_env: array di stringhe "KEY=value"
** - key: nome della variabile da rimuovere (es: "PWD")
**
** Comportamento:
** - Cerca una riga che inizia con "key="
** - Se la trova:
**   - Libera la stringa
**   - Sposta tutte le righe successive indietro (shift sinistro)
**   - Imposta l’ultimo elemento a NULL
** - Bash non segnala errori se la variabile non esiste.
*/
void	unset_variable_2(char **my_env, const char *key)
{
	int		i;
	size_t	key_len;

	i = 0;
	key_len = ft_strlen(key);
	if (!key || !*key)
		return ;
	while (my_env[i])
	{
		// Se trova una variabile che inizia con key + '='
		if (ft_strncmp(my_env[i], key, key_len) == 0 &&
			(my_env[i][key_len] == '=' || my_env[i][key_len] == '\0'))
		{
			free(my_env[i]);
			// Shift a sinistra tutti gli elementi successivi
			while (my_env[i + 1])
			{
				my_env[i] = my_env[i + 1];
				i++;
			}
			my_env[i] = NULL; // ultimo elemento deve diventare NULL
			return ;
		}
		i++;
	}
}
/*
** =====================
** FUNZIONE: unset_variable
** ---------------------
** Scopo: Rimuove una o più variabili passate come argomenti
**        dal my_env gestito dalla minishell.
**
** Parametri:
** - my_env: array delle variabili d’ambiente locali alla shell
** - args: array degli argomenti del comando, es: ["unset", "VAR1", "VAR2",
	NULL]
**
** Comportamento Bash:
** - Se `unset` è chiamato senza argomenti → non fa nulla e ritorna 0
** - Se chiamato con variabili → le rimuove se esistono
** - Non stampa errori se una variabile non esiste
**
** Requisito del subject:
** - Supportare il comando `unset` **senza opzioni**
** - Il comportamento deve essere silenzioso e senza errori
**
** Implementazione:
** - Inizia da `args[1]` perché `args[0]` è il comando `unset`
** - Chiama `unset_variable_2()` per ogni variabile
** - Imposta sempre `g_exit_status = 0`, come fa Bash
*/
int	unset_variable(char **my_env, char **args)
{
	// perchè arg[0] è unset e non una variabile
	int i = 1;

	while (args[i])
	{
		unset_variable_2(my_env, args[i]);
		i++;
	}
	g_exit_status = 0;
	return (1);
}