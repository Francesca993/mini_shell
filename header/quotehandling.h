/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotehandling.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 07:45:06 by francesca         #+#    #+#             */
/*   Updated: 2025/06/07 09:01:01 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUOTEHANDLING_H
# define QUOTEHANDLING_H

#include "envp.h"
#include "parser.h"
#include "quotehandling.h"
#include "minishell.h"

// Forward declaration
typedef struct s_cmd t_cmd;
typedef struct s_pipeline t_pipeline;

char    *remove_single_quote(const char *str);
void    expand_single_quotes(t_cmd *cmd);
void    find_quotes(t_pipeline *pipeline);

extern int g_exit_status;
#endif