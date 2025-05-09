/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:27:42 by francesca         #+#    #+#             */
/*   Updated: 2025/05/09 13:58:19 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/parser.h"

/*
Lexer: Cosa deve fare esattamente
1. Ignora spazi fuori dalle quote
Gli spazi separano i token solo se non sono dentro '...' o "...".
2. Riconosce metacaratteri
Pipe |
Redirezioni: <, >, <<, >>
3. Gestisce quote aperte/chiuse
'...': tutto dentro è un token singolo, senza espansioni.
"...": tutto dentro è un token singolo, ma $VAR va gestito dopo.
🔧 Strategia per implementare il lexer
Scorri il buffer (line) da sinistra verso destra con un indice.
Quando trovi:
Spazio: chiudi il token se non sei dentro le quote.
Quote: entra/esci dal contesto quote.
Metacarattere: chiudi token precedente, poi crea token singolo per |, >, >>, ecc.
Costruisci un token alla volta e salvalo in una lista o array dinamico.
*/

parse_line(line);