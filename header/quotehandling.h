/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotehandling.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 07:45:06 by francesca         #+#    #+#             */
/*   Updated: 2025/06/07 16:22:06 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUOTEHANDLING_H
# define QUOTEHANDLING_H

// Forward declaration
typedef struct s_cmd t_cmd;
typedef struct s_pipeline t_pipeline;

char    *remove_single_quote(const char *str);
void    expand_single_quotes(t_cmd *cmd);
void    find_quotes(t_pipeline *pipeline);
void    expand_quotes(t_pipeline *pipeline);

//extern int g_exit_status;
#endif