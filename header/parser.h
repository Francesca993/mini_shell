/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 13:57:15 by francesca         #+#    #+#             */
/*   Updated: 2025/05/09 14:06:05 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

#include "minishell.h"

typedef struct s_cmd {
    char    **args;       // ["grep", "ciao"]
    char    *infile;      // NULL o "input.txt"
    char    *outfile;     // NULL o "output.txt"
    int     append;       // 1 se >>
    struct s_cmd *next;   // comando successivo nella pipeline
} t_cmd;

#endif
