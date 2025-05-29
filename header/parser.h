/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:54:15 by francesca         #+#    #+#             */
/*   Updated: 2025/05/20 10:21:48 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

#define INITIAL_CAPACITY 16
// ==============================
// ENUM: tipi di token riconosciuti dal lexer
// ==============================
typedef enum e_token_type {
    WORD,
    PIPE,
    REDIR_IN,     // <
    REDIR_OUT,    // >
    APPEND,       // >>
    HEREDOC       // <<
}   t_token_type;

// ==============================
// STRUTTURA di comando singolo
// ==============================
typedef struct s_cmd {
    char    **args;         // ["grep", "ciao", NULL]
    char    *infile;        // NULL o filename
    char    *outfile;       // NULL o filename
    int     append;         // 1 se >>
    int     heredoc;        // 1 se <<
    int     redir_in;       // 1 se <
    int     redir_out;      // 1 se >
    int     pipe;           // 1 se ha pipe in uscita
    int     quote_single;   // 1 se contiene quote singole
    int     quote_double;   // 1 se contiene quote doppie
    int     dollar;         // 1 se contiene $
} t_cmd;

// ==============================
// STRUTTURA della PIPELINE
// ==============================
typedef struct s_pipeline {
    t_cmd           **cmds;     // array di comandi strutturati
    int             n_cmds;     // numero totale di comandi
    char            **tokens;   // token grezzi, es. ["grep", "$VAR", "|", "wc"]
    t_token_type    *types;     // tipo per ciascun token (WORD, PIPE, REDIR_IN, ...)
    int             n_tokens;    // numero totale di token
} t_pipeline;

// ==============================
// LEXER
// ==============================
// Ritorna: numero token, oppure -1 in caso di errore (quote non chiuse ecc.)
int     lexer(char *line, char ***tokens_out, t_token_type **types_out);

// ==============================
// LEXER UTILS
// ==============================

int     is_metachar(char c); // Ritorna 1 se il carattere è un metacarattere della shell: '|', '<' o '>'
int     count_token(const char *line); // Conta quanti token ci sono nella linea di input (considerando quote e metacaratteri)
int     handle_redirection(const char *line, int i, char **tokens, t_token_type *types, int *count); // Gestisce una redirezione (<, >, <<, >>) a partire dalla posizione i
                                                                                        // Aggiunge il token e il tipo negli array e ritorna il nuovo indice
int     fill_tokens(char *line, char **tokens, t_token_type *types); // Popola gli array di token e tipi partendo dalla linea di input
                                                                    // Ritorna il numero di token oppure -1 in caso di errore
int     handle_word(const char *line, int i, char **tokens, t_token_type *types, int *count); // Gestisce una parola normale o quotata a partire dalla posizione i
                                                                                            // Aggiunge il token e tipo e ritorna il nuovo indice (oppure -1 per errore quote)
void    free_partial_tokens(char **tokens, t_token_type *types, int count); // Libera solo i token e tipi allocati fino a count in caso di errore nel lexer
void    exit_shell(int code, const char *msg); // Esce dal programma con codice e messaggio, pulendo la history e settando g_exit_status

// ==============================
// BUILD PIPELINE
// ==============================
t_pipeline *build_pipeline(char **tokens, t_token_type *types, int num_tokens);
void free_pipeline(t_pipeline *pipeline);
void populate_comands(t_pipeline *pipeline); //Popola i comandi della pipeline

// ==============================
// PARSER
// ==============================
t_pipeline   *parse_line(char *line, char **env); // Costruisce array di comandi separati da pipe

// ==============================
// DEBUG
// ==============================
void    print_tokens(char **tokens, t_token_type *types);
void    print_pipeline(t_pipeline *pipeline);

#endif
