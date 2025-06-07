/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:14:07 by francesca         #+#    #+#             */
/*   Updated: 2025/06/07 16:25:25 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

/*
 * Verifica se un carattere è uno spazio bianco valido.
 * Sono considerati spazi:
 *   - spazio ' '
 *   - tabulazione '\t'
 *   - newline '\n'
 *   - vertical tab '\v'
 *   - form feed '\f'
 *   - carriage return '\r'
 *
 * Ritorna:
 * - 1 se il carattere è uno spazio
 * - 0 altrimenti
 */
int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}
/*
 * Verifica se un carattere è un metacarattere della shell.
 * Sono considerati metacaratteri:
 *   - '|': pipe
 *   - '<': redirezione input
 *   - '>': redirezione output
 *
 * Ritorna:
 * - 1 se il carattere è un metacarattere
 * - 0 altrimenti
 */
int	is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/*
 * Conta quanti token sono presenti nella linea di input.
 * I token includono:
 *   - parole (comandi, argomenti)
 *   - redirezioni (<, >, <<, >>)
 *   - pipe (|)
 *
 * Gestisce anche le quote:
 *   - quote singole e doppie vengono considerate parte del token
 *   - se una quote non viene chiusa,
	il lexer si interrompe (resta in uno stato errato)
 *
 * Ritorna:
 * - Numero totale di token nella linea
 */
int	count_token(const char *line)
{
	int	i;
	int	count;
	int	quote;

	count = 0; // numero di token trovati
	quote = 0; // stato quote attivo (' o "), 0 se non dentro quote
	i = 0;
	while (line[i])
	{
		if (line[0] == '|')
		{
			g_exit_status = 1;
			return (exit_shell(g_exit_status, "parse error near `|'"), - 1);
		}
		// Salta spazi iniziali
		while (ft_isspace(line[i]))
			i++;
		// Fine stringa dopo spazi
		if (!line[i])
			break ;
		// === METACARATTERI ===
		// Pipe: è sempre un token singolo
		if (line[i] == '|')
		{
			count++;
			i++;
		}
		// Redirezioni: può essere singolo (< >) o doppio (<< >>)
		else if (line[i] == '<' || line[i] == '>')
		{
			i++;
			if (line[i] == line[i - 1]) // >> o <<
				i++;
			count++; //un solo token per qualsiasi redirezione
		}
		// === PAROLA / ARGOMENTO / QUOTED TOKEN ===
		else
		{
			count++; // inizia un nuovo token
			// Avanza finché:
			// - sei dentro quote
			// - oppure non incontri spazio o metacarattere
			while (line[i] && (quote || (!is_metachar(line[i])
						&& !ft_isspace(line[i]))))
			{
				// Se trovi una quote aperta (solo se non sei già dentro un'altra quote)
				if ((line[i] == '\'' || line[i] == '"') && !quote)
					quote = line[i]; // entra nella quote
				else if (line[i] == quote)
					quote = 0; // chiudi quote
				i++;
			}
		}
	}
	return (count);
}

/*
 * Gestisce la creazione di un token di redirezione a partire da `line[i]`.
 * Riconosce:
 *   - '>'     → REDIR_OUT
 *   - '>>'    → APPEND
 *   - '<'     → REDIR_IN
 *   - '<<'    → HEREDOC
 *
 * Scrive il token corrispondente in `tokens[*count]` e aggiorna il tipo.
 *
 * Parametri:
 * - line: linea originale
 * - i: indice corrente nella linea
 * - tokens: array dei token
 * - types: array dei tipi di token
 * - count: indice del token corrente (passato per riferimento)
 *
 * Ritorna:
 * - Nuova posizione `i` dopo aver letto il token di redirezione
 */
int	handle_redirection(const char *line, int i, char **tokens,
		t_token_type *types, int *count)
{
	char	c;

	/*
	c = line[i];
	i++;
    */
	c = line[i++];
	if (line[i] == c) // >> o <<
	{
		tokens[*count] = ft_substr(line, i - 1, 2);
		if (c == '>')
			types[*count] = APPEND;
		else
			types[*count] = HEREDOC;
		i++;
	}
	else
	{
		tokens[*count] = ft_substr(line, i - 1, 1);
		if (c == '>')
			types[*count] = REDIR_OUT;
		else
			types[*count] = REDIR_IN;
	}
	(*count)++;
	return (i);
}

/*
 * Gestisce un token di tipo WORD o stringa tra quote.
 *
 * Un token è considerato una parola finché:
 *   - non è un metacarattere
 *   - non è uno spazio
 *   - oppure finché si è dentro a delle quote (singole o doppie)
 *
 * Se viene trovata una quote non chiusa, ritorna -1 per segnalare errore.
 * In caso contrario, salva il token e aggiorna `types[*count]`.
 *
 * Parametri:
 * - line: linea di input
 * - i: indice iniziale
 * - tokens: array di stringhe da riempire
 * - types: array parallelo dei tipi
 * - count: puntatore al numero corrente di token
 *
 * Ritorna:
 * - Nuovo indice i dopo il token
 * - -1 in caso di errore (quote non chiusa)
 */
int	handle_word(const char *line, int i, char **tokens, t_token_type *types,
		int *count)
{
	int		start;
	char	quote;

	start = i;
	quote = 0;
	while (line[i] && (quote || (!is_metachar(line[i])
				&& !ft_isspace(line[i]))))
	{
		if ((line[i] == '\'' || line[i] == '"') && !quote)
			quote = line[i];
		else if (line[i] == quote)
			quote = 0;
		i++;
	}
	if (quote)
		return (-1); // errore: quote non chiusa
	tokens[*count] = ft_substr(line, start, i - start);
	types[*count] = WORD;
	(*count)++;
	return (i);
}

/*
 * Scorre l'intera linea di input e popola gli array `tokens` e `types`.
 * Utilizza `handle_redirection()` e `handle_word()` per costruire i token.
 *
 * Token riconosciuti:
 *   - PIPE (|): copiato direttamente
 *   - REDIREZIONI (<, >, <<, >>): gestite con `handle_redirection()`
 *   - WORD: parole generiche, gestite con `handle_word()`
 *
 * Parametri:
 * - line: stringa da analizzare
 * - tokens: array dove salvare i token estratti
 * - types: array parallelo con i tipi di token
 *
 * Ritorna:
 * - Numero di token trovati
 * - -1 se c'è un errore (quote non chiusa, da `handle_word`)
 */
int	fill_tokens(char *line, char **tokens, t_token_type *types)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i])
	{
		while (ft_isspace(line[i]))
			i++;
		if (!line[i])
			break ;
		// --- PIPE
		if (line[i] == '|')
		{
			tokens[count] = ft_strdup("|");
			types[count++] = PIPE;
			i++;
		}
		// --- REDIREZIONI
		else if (line[i] == '<' || line[i] == '>')
			i = handle_redirection(line, i, tokens, types, &count);
		// --- WORD o QUOTED STRING
		else
		{
			i = handle_word(line, i, tokens, types, &count);
			if (i == -1)
				return (-1); // errore da handle_word (es. quote non chiusa)
		}
	}
	return (count);
}
