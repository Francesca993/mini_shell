/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:27:42 by francesca         #+#    #+#             */
/*   Updated: 2025/05/19 09:01:08 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/parser.h"

/*
// ==============================
// Obiettivi del lexer
// ==============================
// Divide la riga (line) in token:
// parole (comandi e argomenti)
// metacaratteri: |, <, >, >>, <<
// gestisce correttamente '...' e "..."
// Ritorna:
// tokens_out → array di stringhe (terminato da NULL)
// types_out → array parallelo di t_token_type
// ==============================
*/

void print_tokens(char **tokens, t_token_type *types)
{
    int i = 0;

    printf("=== TOKENS ===\n");
    while (tokens[i])
    {
        printf("[%s] -> ", tokens[i]);
        switch (types[i])
        {
            case WORD: printf("WORD\n"); break;
            case PIPE: printf("PIPE\n"); break;
            case REDIR_IN: printf("REDIR_IN (<)\n"); break;
            case REDIR_OUT: printf("REDIR_OUT (>)\n"); break;
            case APPEND: printf("APPEND (>>)\n"); break;
            case HEREDOC: printf("HEREDOC (<<)\n"); break;
            default: printf("UNKNOWN\n"); break;
        }
        i++;
    }
    printf("==============\n");
}
/*
// Riceve la line da readline
// La passa al lexer()
// Ottiene tokens[] e types[]
// (Per ora) stampa solo i risultati del lexer, senza ancora costruire t_pipeline
*/
t_pipeline   *parse_line(char *line, char **env)
{
    (void)env; // Per ora lo ignoriamo
    
    char **tokens = NULL;
    t_token_type *types = NULL;
    int ntokens;
    t_pipeline *pipeline;

    ntokens = lexer(line, &tokens, &types);
    if (ntokens == -1)
    {
        fprintf(stderr, "Lexer error\n");
        g_exit_status = 2;
        return NULL;
    }
    // Debug temporaneo
    print_tokens(tokens, types);
     // ✅ Costruisce la pipeline da tokens e types
    pipeline = build_pipeline(tokens, types, ntokens);
    if (!pipeline)
    {
         fprintf(stderr, "Parser error\n");
         g_exit_status = 2;
         return NULL;
    }
    return (pipeline);
    
}

