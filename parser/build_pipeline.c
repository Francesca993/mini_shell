/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 08:21:25 by francesca         #+#    #+#             */
/*   Updated: 2025/06/20 18:17:40 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

/*
 * Conta quanti comandi sono presenti nella linea, basandosi sulle pipe (|).
 * Ogni pipe separa due comandi, quindi il numero totale è 1 + numero di pipe.
 *
 * Esempio:
 * Input: echo hello | grep test > out.txt
 * Ritorna: 2
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
 * Costruisce la struttura `t_pipeline` a partire dai token e dai loro tipi.
 *
 * Passaggi:
 * 1. Conta quanti comandi ci sono (basato sulle pipe).
 * 2. Alloca la struttura `t_pipeline` e l'array di comandi `t_cmd**`.
 * 3. Assegna token, tipi, e dimensioni totali alla pipeline.
 * 4. Chiama `populate_comands()` per popolare ogni `t_cmd` all'interno della pipeline.
 *
 * Ritorna:
 * - Puntatore alla `t_pipeline` allocata e popolata.
 * - NULL in caso di errore di allocazione.
 */
t_pipeline *build_pipeline(char **tokens, t_token_type *types, int num_tokens, t_pipeline *pipeline, char **env)
{
    int num_cmds;

    num_cmds = count_cmds(tokens);
    // printf("num_cmds: %d\n", num_cmds);

    // Allocazione array di puntatori a comandi
    pipeline->cmds = ft_calloc(num_cmds + 1, sizeof(t_cmd *));
    pipeline->types = types;
    pipeline->tokens = tokens;
    pipeline->n_cmds = num_cmds;
    pipeline->n_tokens = num_tokens;
    pipeline->my_env = env;
    
    // printf("%d\n", pipeline->n_cmds);
    // printf("%d\n", pipeline->n_tokens);

    populate_comands(pipeline);
    find_quotes(pipeline);
    expand_pipeline_variables(pipeline);
    // process_pipeline(pipeline);
    return (pipeline);
}

/*
 * Conta quanti token di tipo WORD appartengono a un singolo comando.
 * Serve per allocare correttamente `cmd->args`.
 *
 * Scorre i token a partire da `start` fino a una PIPE o fine input.
 * Salta i token che sono parte di redirezioni (cioè token successivi a <, >, >>, <<).
 *
 * Parametri:
 * - types: array di tipi dei token.
 * - start: indice da cui iniziare a contare.
 * - n_tokens: numero totale di token.
 *
 * Ritorna:
 * - Numero di argomenti (WORD) per il comando corrente.
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

/*
 * Popola l'array di comandi `pipeline->cmds` analizzando token e tipi.
 *
 * Per ogni comando:
 * - Conta i WORD per allocare `cmd->args`.
 * - Alloca e riempie la struttura `t_cmd`:
 *   - `args[]`: solo token di tipo WORD (duplicati con `ft_strdup`).
 *   - Redirezioni:
 *     - <    → `infile`, `redir_in`
 *     - >    → `outfile`, `redir_out`
 *     - >>   → `outfile`, `append`
 *     - <<   → `infile`, `heredoc`
 * - Se è presente una PIPE, imposta `cmd->pipe = 1` e passa al prossimo comando.
 *
 * Nota: termina se termina l'array di token.
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
        cmd->fd_in = -1;
        cmd->fd_out = -1;
        if (i < pipeline->n_tokens && pipeline->types[i] == PIPE)
        {
            cmd->pipe = 1;
            i++; // salta la pipe
        }
        pipeline->cmds[cmd_idx++] = cmd;
    }
    find_quotes(pipeline);
    expand_pipeline_variables(pipeline);
}

