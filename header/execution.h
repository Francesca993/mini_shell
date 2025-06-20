/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 09:45:03 by francesca         #+#    #+#             */
/*   Updated: 2025/06/20 18:08:46 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H
# include "parser.h"

// ==============================
// PROCESSING PIPELINE
// ==============================
int		process_pipeline(t_pipeline *pipeline, char ***main_env);
int		process_args(t_cmd *cmd);

// ==============================
// EXECUTE PIPELINE
// ==============================
void	execute_pipeline(t_pipeline *pipeline);

#endif