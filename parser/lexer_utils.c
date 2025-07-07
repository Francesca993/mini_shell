/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:14:07 by francesca         #+#    #+#             */
/*   Updated: 2025/07/06 23:02:19 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

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

	count = 0;
	quote = 0;
	i = 0;
	while (line[i])
	{
		while (ft_isspace(line[i]))
			i++;
		if (!line[i])
			break ;
		if (line[i] == '|')
		{
			count++;
			i++;
		}
		else if (line[i] == '<' || line[i] == '>')
		{
			i++;
			if (line[i] == line[i - 1])
				i++;
			count++;
		}
		else
		{
			count++;
			while (line[i] && (quote || (!is_metachar(line[i])
						&& !ft_isspace(line[i]))))
			{
				if ((line[i] == '\'' || line[i] == '"') && !quote)
					quote = line[i];
				else if (line[i] == quote)
					quote = 0;
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

	c = line[i++];
	if (line[i] == c)
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
	int		escaped;

	start = i;
	quote = 0;
	escaped = 0;
	while (line[i] && (quote || (!is_metachar(line[i])
				&& !ft_isspace(line[i]))))
	{
		escaped = (i > 0 && line[i - 1] == '\\');
		if ((line[i] == '"' || line[i] == '\'') && !quote)
		{
			if (!(line[i] == '"' && escaped))
				quote = line[i];
		}
		else if (line[i] == quote && !(quote == '"' && escaped))
			quote = 0;
		else if (line[i] == '\\')
		{
			if (!line[i + 1])
				return (exit_shell(2, "Error backslah not closed \n"), -1);
			if ((line[i + 1] == ' ' || line[i + 1] == '\0') && (!line[i + 2]
					|| ft_isspace(line[i + 2])))
				return (exit_shell(2, "Error backslah not closed \n"), -1);
			i++;
		}
		i++;
	}
	if (quote)
		return (exit_shell(2, "syntax error: unclosed quote \n"), -1);
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
		if (line[i] == '|')
		{
			tokens[count] = ft_strdup("|");
			types[count++] = PIPE;
			i++;
		}
		else if (line[i] == '<' || line[i] == '>')
			i = handle_redirection(line, i, tokens, types, &count);
		else
		{
			i = handle_word(line, i, tokens, types, &count);
			if (i == -1)
				return (-1);
		}
	}
	return (count);
}
