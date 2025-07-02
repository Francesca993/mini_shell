/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 09:45:03 by francesca         #+#    #+#             */
/*   Updated: 2025/06/30 14:55:31 by skayed           ###   ########.fr       */
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
int		setup_redir_in(t_cmd *cmd);
int		setup_redir_out(t_cmd *cmd);
int		setup_redir_append(t_cmd *cmd);
int		setup_heredoc(t_cmd *cmd, char *delimiter);

#endif