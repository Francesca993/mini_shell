/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 09:45:03 by francesca         #+#    #+#             */
/*   Updated: 2025/07/07 08:43:02 by skayed           ###   ########.fr       */
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

// ==============================
// EXEC UTILS
// ==============================

void	setup_pipes(t_pipeline *pipeline, int i, int **pipes);
void	try_execute_builtin(t_cmd *cmd, char ***env);
void	exec_with_env_path(t_cmd *cmd, char **env);
int		fork_and_exec(t_pipeline *pipeline, int i, int **pipes, pid_t *pids);
void	wait_all(pid_t *pids, int n_cmds);

#endif