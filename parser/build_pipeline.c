/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 08:21:25 by francesca         #+#    #+#             */
/*   Updated: 2025/05/29 09:37:54 by francesca        ###   ########.fr       */
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
static int count_cmds(char ** tokens)
{
    int count = 1; // almeno un comando esiste
    int i = 0;

    while (tokens[i])
    {
        if (ft_strncmp(tokens[i], "|", 2) == 0)
            count++;
        i++;
    }
    return (count);
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

t_pipeline *build_pipeline(char **tokens, t_token_type *types, int num_tokens)
{
    int num_cmds;

    num_cmds = count_cmds(tokens);
    // printf("num_cmds: %d\n", num_cmds);

    // Allocazione array di puntatori a comandi
    t_pipeline *pipeline = ft_calloc(1, sizeof(t_pipeline));
    if (!pipeline)
        return NULL; // errore allocazione
    pipeline->cmds = ft_calloc(num_cmds + 1, sizeof(t_cmd *));
    pipeline->types = types;
    pipeline->tokens = tokens;
    pipeline->n_cmds = num_cmds;
    pipeline->n_tokens = num_tokens;
    
    // printf("%d\n", pipeline->n_cmds);
    // printf("%d\n", pipeline->n_tokens);

    populate_comands(pipeline);

    return (pipeline);
}
/*
Cosa fa:
Parte da types[start]
Conta solo i WORD
Si ferma quando trova una PIPE o fine array (types[i] == 0)
*/
static int count_args_for_cmds(t_token_type *types, int start, int n_tokens)
{
    int count = 0;
    int i = start;

    while (i < n_tokens && types[i] != PIPE)
    {
        if (types[i] == WORD)
        {
            count++;
        }
        else if ((types[i] == REDIR_IN || types[i] == REDIR_OUT ||
                  types[i] == APPEND || types[i] == HEREDOC) &&
                 i + 1 < n_tokens)
        {
            i++; // salta filename
        }
        i++;
    }
    return count;
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
Quindi in expand_pipeline():

Tracciare quali token sono in quote singole/doppie
Espandere solo se:
Non sono in quote singole
Oppure sono in quote doppie e contengono $
*/

/*

Riceve tokens, types, n_tokens, pipeline
Per ogni comando (tra PIPE):
Conta quanti WORD ci sono (usando count_args_for_cmds)
Alloca t_cmd
Alloca args correttamente
Riempie args, infile, outfile, redir_in, append, ecc.
Inserisce t_cmd in pipeline->cmds[cmd_idx++]
// Ogni t_cmd ha:
// args (comando e argomenti)
// infile, outfile
// append, heredoc
populate_cmds deve:
Contare quanti WORD ci sono fino alla prossima PIPE
Allocare il t_cmd
Allocare cmd->args con la dimensione esatta
Copiare i token (WORD) dentro args con ft_strdup
Gestire eventuali redirezioni (<, >, >>, <<)
Passare al comando successivo se trova PIPE
*/
void populate_comands(t_pipeline *pipeline)
{
    int i = 0;
    int cmd_idx = 0;
    // for (int j = i; j < pipeline->n_tokens && pipeline->types[j] != PIPE; j++) 
    // {
    //     printf("[TOKEN DEBUG] j=%d, type=%d, token='%s'\n", j, pipeline->types[j], pipeline->tokens[j]);
    // }    

    while (i < pipeline->n_tokens)
    {
        int n_args = count_args_for_cmds(pipeline->types, i, pipeline->n_tokens);
        // printf("[DEBUG] cmd_idx=%d, i=%d, n_args=%d\n", cmd_idx, i, n_args);

        // Alloca nuovo comando
        t_cmd *cmd = ft_calloc(1, sizeof(t_cmd));
        if (!cmd)
            return;

        cmd->args = ft_calloc(n_args + 1, sizeof(char *));
        if (!cmd->args)
        {
            free(cmd);
            return;
        }

        int arg_idx = 0;
        while (i < pipeline->n_tokens && pipeline->types[i] != PIPE)
        {
            if (pipeline->types[i] == WORD)
                cmd->args[arg_idx++] = ft_strdup(pipeline->tokens[i]);
            else if (pipeline->types[i] == REDIR_IN && i + 1 < pipeline->n_tokens)
                cmd->infile = ft_strdup(pipeline->tokens[++i]), cmd->redir_in = 1;
            else if (pipeline->types[i] == REDIR_OUT && i + 1 < pipeline->n_tokens)
                cmd->outfile = ft_strdup(pipeline->tokens[++i]), cmd->redir_out = 1;
            else if (pipeline->types[i] == APPEND && i + 1 < pipeline->n_tokens)
                cmd->outfile = ft_strdup(pipeline->tokens[++i]), cmd->append = 1;
            else if (pipeline->types[i] == HEREDOC && i + 1 < pipeline->n_tokens)
                cmd->infile = ft_strdup(pipeline->tokens[++i]), cmd->heredoc = 1;

            i++;
        }

        cmd->args[arg_idx] = NULL;

        if (i < pipeline->n_tokens && pipeline->types[i] == PIPE)
        {
            cmd->pipe = 1;
            i++; // salta la pipe
        }

        pipeline->cmds[cmd_idx++] = cmd;
    }
    
}

