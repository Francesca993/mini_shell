/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:14:07 by francesca         #+#    #+#             */
/*   Updated: 2025/05/13 22:23:19 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/parser.h"

int ft_isspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n'
        || c == '\v' || c == '\f' || c == '\r');
}
/*
Controlla se un carattere è un metacarattere della shell, ovvero uno di:
'|' (pipe)
'<' (redirezione input)
'>' (redirezione output)
ritorna
1 (cioè true) se c è uno dei tre simboli sopra
0 (cioè false) se c è qualunque altro carattere
is_metachar('|');   // → 1
is_metachar('>');   // → 1
is_metachar('<');   // → 1
is_metachar('a');   // → 0
is_metachar(' ');   // → 0
*/
int  is_metachar(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

/*
 * Conta il numero di token presenti nella linea di input.
 * I token possono essere:
 *   - parole (comandi o argomenti)
 *   - redirezioni (<, >, <<, >>)
 *   - pipe (|)
 */
int count_token(const char *line)
{
    int i;
    int count;
    int quote;
    count = 0;       // numero di token trovati
    quote = 0;       // stato quote attivo (' o "), 0 se non dentro quote
    i = 0;
    while(line[i])
    {
        // Salta spazi iniziali
        while(ft_isspace(line[i]))
            i++;
        // Fine stringa dopo spazi
        if (!line[i])
            break;
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
            if (line[i] == line[i -1])// >> o <<
                i++;
            count++; //un solo token per qualsiasi redirezione
        }
        // === PAROLA / ARGOMENTO / QUOTED TOKEN ===
        else
        {
            count++;  // inizia un nuovo token
            // Avanza finché:
			// - sei dentro quote
			// - oppure non incontri spazio o metacarattere
            while (line[i] && (quote || (!is_metachar(line[i]) && !ft_isspace(line[i]))))
            {
                // Se trovi una quote aperta (solo se non sei già dentro un'altra quote)
                if((line[i] == '\'' || line[i] == '"') && !quote)
                    quote = line[i];  // entra nella quote
				else if (line[i] == quote)
					quote = 0; // chiudi quote
				i++;
            }
        }
    }
    return (count);
}

/*
Legge da line[i]
Riconosce >, >>, <, <<
Scrive nella posizione tokens[*count] e types[*count]
Incrementa *count e restituisce la nuova posizione i
*/
int handle_redirection(const char *line, int i, char **tokens, t_token_type *types, int *count)
{
    /*
    char c;
	c = line[i];
	i++;
    */
    char c = line[i++];
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

#include "parser.h"

int handle_word(const char *line, int i, char **tokens, t_token_type *types, int *count)
{
	int start = i;
	char quote = 0;

	while (line[i] && (quote || (!is_metachar(line[i]) && !ft_isspace(line[i]))))
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

int    fill_tokens(char *line, char **tokens, t_token_type *types)
{
    int i = 0;
    int count = 0;
    while (line[i])
    {
        while (ft_isspace(line[i]))
            i++;
        if (!line[i])
            break;
        // --- PIPE
        if(line[i] == '|')
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
				return -1;  // errore da handle_word (es. quote non chiusa)
        }
    }
    return (count);
}

