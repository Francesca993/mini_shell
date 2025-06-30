/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:03:26 by francesca         #+#    #+#             */
/*   Updated: 2025/06/30 08:34:48 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

/*
 * Funzione principale del lexer: analizza una linea di input e produce
 * un array di token e un array parallelo con i tipi corrispondenti.
 *
 * Flusso:
 * 1. Conta quanti token saranno presenti (con `count_token()`).
 * 2. Alloca dinamicamente due array:
 *    - `tokens`: array di stringhe per ogni token
 *    - `types`: array di `t_token_type` con i tipi (es. WORD, PIPE, ecc.)
 * 3. Chiama `fill_tokens()` per popolare gli array.
 * 4. In caso di errore (es. quote non chiusa), stampa un messaggio,
 *    imposta il codice di uscita globale (`g_exit_status = 2`) e libera la memoria.
 *
 * Parametri:
 * - line: la linea di input da analizzare
 * - tokens_out: doppio puntatore dove salvare l'array dei token estratti
 * - types_out: doppio puntatore dove salvare l'array dei tipi corrispondenti
 *
 * Ritorna:
 * - Numero di token trovati e validi
 * - -1 in caso di errore (es. errore di malloc o quote non chiusa)
 */
int lexer(char *line, char ***tokens_out, t_token_type **types_out)
{
    int n_tokens;
    char **tokens;
    t_token_type *types;
    int filled;
    
    n_tokens = count_token(line);
    tokens = (char**)ft_calloc(n_tokens + 1, sizeof(char *));
    types = ft_calloc(n_tokens, sizeof(t_token_type));
	if (!tokens || !types)
	{
		free(tokens);
		free(types);
		exit_shell(1, NULL);
		return -1;
	}
	filled = fill_tokens(line, tokens, types);
	if (filled == -1)
	{
		// exit_shell(2, "syntax error: unclosed quote\n"); gia presente in un altra funzione nn serve qui
		// fprintf(stderr, "syntax error: unclosed quote\n");
		// g_exit_status = 2;
		free_partial_tokens(tokens, types, n_tokens);
		return (-1);
	}
	*tokens_out = tokens;
	*types_out = types;
	return (filled);
}
