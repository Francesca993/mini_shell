/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:54:15 by francesca         #+#    #+#             */
/*   Updated: 2025/05/13 12:07:29 by francesca        ###   ########.fr       */
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
    char    **args;      // ["grep", "ciao", NULL]
    char    *infile;     // NULL o filename
    char    *outfile;    // NULL o filename
    int     append;      // 1 se >>
    int     heredoc;     // 1 se <<
}   t_cmd;

// ==============================
// LEXER
// ==============================
// Ritorna: numero token, oppure -1 in caso di errore (quote non chiuse ecc.)
int     lexer(char *line, char ***tokens_out, t_token_type **types_out);

// ==============================
// PARSER
// ==============================
// Costruisce array di comandi separati da pipe
t_cmd   **parse_line(char *line, char **env);

// ==============================
// DEBUG
// ==============================
void    print_tokens(char **tokens, t_token_type *types);
void    print_pipeline(t_cmd **pipeline);

#endif
