/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 08:01:34 by skayed            #+#    #+#             */
/*   Updated: 2025/07/07 08:51:02 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

void	setup_pipes(t_pipeline *pipeline, int i, int **pipes)
{
	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < pipeline->n_cmds - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	close_pipes(pipes, pipeline->n_cmds - 1);
}

void	try_execute_builtin(t_cmd *cmd, char ***env)
{
	if (is_builtin(cmd))
	{
		execute_builtin(cmd, env, NULL);
		exit(1);
	}
}

void	exec_with_env_path(t_cmd *cmd, char **env)
{
	char	*path;

	path = check_path(find_path(env), cmd->args[0]);
	if (!path)
		exit(127);
	execve(path, cmd->args, env);
	perror("execve");
	free(path);
	exit(1);
}

void	wait_all(pid_t *pids, int n_cmds)
{
	int	i;
	int	status;

	i = 0;
	while (i < n_cmds)
		waitpid(pids[i++], &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else
		g_exit_status = 1;
}
