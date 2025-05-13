/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:14:07 by francesca         #+#    #+#             */
/*   Updated: 2025/05/13 12:33:03 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
static int  is_metachar(char c)
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
            while (line[i] &&(quote || !is_metachar(line[i]) && !ft_isspace(line[i])))
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

