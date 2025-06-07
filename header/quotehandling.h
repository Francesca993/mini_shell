/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotehandling.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 07:45:06 by francesca         #+#    #+#             */
/*   Updated: 2025/06/07 20:55:47 by skayed           ###   ########.fr       */
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
char    *handle_backslash(char *str);

//extern int g_exit_status;
#endif