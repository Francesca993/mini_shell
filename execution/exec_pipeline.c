/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:22:22 by skayed            #+#    #+#             */
/*   Updated: 2025/07/04 17:36:04 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

static int	**create_pipes(int n_cmds)
{
	int	**pipes;
	int	i;

	i = 0;
	pipes = malloc(sizeof(int *) * (n_cmds - 1));
	if (!pipes)
		return (NULL);
	while (i < n_cmds - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
			return (perror("malloc failed"), NULL);
		if (pipe(pipes[i]) < 0)
			return (perror("pipe failed"), NULL);
		i++;
	}
	return (pipes);
}

static void	close_pipes(int **pipes, int n_pipes)
{
	int	i;

	i = 0;
	while (i < n_pipes)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}
static void	execute_cmd(t_cmd *cmd, int i, int **pipes, int n_cmds,
		char **my_env)
{
	char	*path;

	// Se non è il primo comando, duplica la pipe precedente su stdin
	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	// Se non è l'ultimo comando, duplica la pipe attuale su stdout
	if (i < n_cmds - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	// Chiudi tutte le pipe nel figlio
	for (int j = 0; j < n_cmds - 1; j++)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
	}
	set_redirections(cmd);
	if (is_builtin(cmd))
	{
		execute_builtin(cmd, &my_env, NULL);
		exit(1);
	}
	if (ft_strchr(cmd->args[0], '/'))
	{
		if (access(cmd->args[0], X_OK) == 0)
			execve(cmd->args[0], cmd->args, my_env);
		else
			exit(126);
	}
	else
	{
		path = check_path(find_path(my_env), cmd->args[0]);
		if (!path)
			exit(127);
		execve(path, cmd->args, my_env);
		perror("execve");
		free(path);
		exit(1);
	}
}

void	execute_pipeline(t_pipeline *pipeline)
{
	int		i;
	pid_t	*pids;
	int		**pipes;

	i = 0;
	pipes = create_pipes(pipeline->n_cmds);
	if (!pipes)
		return ;
	pids = malloc(sizeof(pid_t) * pipeline->n_cmds);
	if (!pids)
		return ;
	while (i < pipeline->n_cmds)
	{
		pids[i] = fork();
		if (pids[i] < 0)
			return (perror("Pipe failed"));
		if (pids[i] == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			execute_cmd(pipeline->cmds[i], i, pipes, pipeline->n_cmds,
					pipeline->my_env);
		}
		i++;
	}
	close_pipes(pipes, pipeline->n_cmds - 1);
	i = 0;
	while (i < pipeline->n_cmds)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
	free(pipes);
	free(pids);
}
