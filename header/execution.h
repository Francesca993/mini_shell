/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 09:45:03 by francesca         #+#    #+#             */
/*   Updated: 2025/06/12 09:47:35 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H
#include "parser.h"

// ==============================
// PROCESSING PIPELINE
// ==============================
int process_pipeline(t_pipeline *pipeline);
int process_args(t_cmd *cmd);

#endif