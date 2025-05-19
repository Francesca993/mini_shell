/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 08:21:25 by francesca         #+#    #+#             */
/*   Updated: 2025/05/19 09:01:19 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/parser.h"

/*
* Conta i comandi
// count_cmds()
// Conta solo i comandi → 1 + numero di pipe
// Nell’esempio sopra: 2 comandi:
// echo hello
// grep test > out.txt
*/
static int count_cmds(t_token_type *types)
{
    int count = 1; // almeno un comando esiste
    int i = 0;

    while (types[i])
    {
        if (types[i] == PIPE)
            count++;
        i++;
    }
    return count;
}

/*
* Costruire un array di t_cmd* (uno per ogni comando separato da pipe) a partire dai token e dai loro tipi.
* Scorre i tokens e types
* Ogni volta che incontra una PIPE o la fine, chiude il comando corrente
* Riempie la struttura t_cmd con:
* args: array dinamico di stringhe
* infile, outfile, append, heredoc (se ci sono redirezioni)
*
Conta i comandi
Alloca il pipeline
Per ogni t_cmd, popola:
args: solo WORD
infile, outfile, append, heredoc: da redirezioni
Gestisce le pipe come separatori
Gestisce errori di allocazione
*/

t_pipeline *build_pipeline(char **tokens, t_token_type *types, int n_tokens)
{
    int n_cmds;

    n_cmds = count_cmds(types);
    (void)n_tokens; // sopprime warning per ora

    // Allocazione array di puntatori a comandi
    t_pipeline *pipeline = ft_calloc(1, sizeof(t_pipeline));
    if (!pipeline)
        return NULL; // errore allocazione
    pipeline->cmds = ft_calloc(n_cmds + 1, sizeof(t_cmd *));
    pipeline->types = types;
    pipeline->tokens = tokens;

    // TODO: Popolare i t_cmd

    return (pipeline);
}

// Ogni t_cmd ha:
// args (comando e argomenti)
// infile, outfile
// append, heredoc

/*
Dal PDF:

Handle ’ (single quote): disabilita interpretazione
Handle " (double quote): interpreta solo $
Handle $VAR
Handle $? → ultimo exit status
Quindi in expand_pipeline() dovrai:

Tracciare quali token sono in quote singole/doppie
Espandere solo se:
Non sono in quote singole
Oppure sono in quote doppie e contengono $
*/