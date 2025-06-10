/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotehandling.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 07:45:06 by francesca         #+#    #+#             */
/*   Updated: 2025/06/10 17:34:33 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUOTEHANDLING_H
# define QUOTEHANDLING_H

// Forward declaration
typedef struct s_cmd t_cmd;
typedef struct s_pipeline t_pipeline;

char    *remove_quotes(const char *str);
void    expand_single_quotes(t_cmd *cmd);
void    find_quotes(t_pipeline *pipeline);
void    expand_quotes(t_pipeline *pipeline);
//char    *handle_backslash(char *str);
char    *handle_backslash(char *str, int *dollar);

//Gestione backshalsh
char    *handle_backslash_outside_quotes(const char *str);

//Processing argoments
void process_pipeline(t_pipeline *pipeline);
void process_args(t_cmd *cmd);

#endif