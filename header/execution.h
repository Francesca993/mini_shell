/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 09:45:03 by francesca         #+#    #+#             */
/*   Updated: 2025/06/24 12:07:47 by skayed           ###   ########.fr       */
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
void	set_redirections(t_cmd *cmd);
char	*find_path(char **envp);
char	*check_path(char *env_path, char *cmd);

#endif