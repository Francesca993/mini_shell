/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:22:22 by skayed            #+#    #+#             */
/*   Updated: 2025/06/24 12:14:22 by skayed           ###   ########.fr       */
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
	while (i < n_cmds)
	{
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
	}
	free(pipes);
}
static void	execute_cmd(t_cmd *cmd, int i, int **pipes, int n_cmds,
		char **my_env)
{
	char	*path;

	if (i > 0 && !cmd->redir_in && !cmd->heredoc)
		// se non sei il primo e non hai un redir di input allora il tuo input viene dalla pipe precedente
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (cmd->pipe && !cmd->redir_out && !cmd->append)
		// se hai pipe in uscita e non hai redir su file, allora la pipe successiva riceve stdout
			dup2(pipes[i][1], STDOUT_FILENO);
	close_pipes(pipes, (n_cmds - 1));
	set_redirections(cmd);
	if (ft_strchr(cmd->args[0], '/'))
	{
		if (access(cmd->args[0], X_OK) == 0)
			execve(cmd->args[0], cmd->args, my_env);
		else
		{
			fprintf(stderr, "minishell: %s: No such file or permission denied\n", cmd->args[0]);
			exit(126);
		}
	}
	else
	{
		path = check_path(find_path(my_env), cmd->args[0]);
		if (!path)
		{
			fprintf(stderr, "minishell: %s: command not found\n", cmd->args[0]);
			exit(127);
		}
		execve(path, cmd->args, my_env);
		perror("execve");
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
	i = 0;
	while (i < pipeline->n_cmds)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
	free(pids);
}
