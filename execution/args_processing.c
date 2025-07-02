/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_processing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 15:20:58 by francesca         #+#    #+#             */
/*   Updated: 2025/07/02 13:33:32 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int	process_args(t_cmd *cmd)
{
	int		i;
	char	*tmp;

	i = 0;
	while (cmd->args && cmd->args[i])
	{
		if (!cmd->quote_single && !cmd->quote_double)
		{
			tmp = handle_backslash_outside_quotes(cmd->args[i]);
			free(cmd->args[i]);
			cmd->args[i] = tmp;
		}
		i++;
	}
	return (1);
}

static int	process_single_cmd(t_pipeline *pipeline, char ***main_env)
{
	int	exit;
	int	old_stdin;
	int	old_stdout;

	exit = 1;
	process_args(pipeline->cmds[0]);
	old_stdin = dup(STDIN_FILENO);
	old_stdout = dup(STDOUT_FILENO);
	set_redirections(pipeline->cmds[0]);
	exit = execute_builtin(pipeline->cmds[0], &pipeline->my_env, pipeline);
	dup2(old_stdin, STDIN_FILENO);
	dup2(old_stdout, STDOUT_FILENO);
	close(old_stdin);
	close(old_stdout);
	if (main_env && *main_env != pipeline->my_env)
		*main_env = pipeline->my_env;
	return (exit);
}
int	process_pipeline(t_pipeline *pipeline, char ***main_env)
{
	int	j;

	j = 0;
	if (pipeline->n_cmds == 1 && is_builtin(pipeline->cmds[0]))
	{
		return (process_single_cmd(pipeline, main_env));
	}
	while (pipeline->cmds[j])
	{
		process_args(pipeline->cmds[j]);
		j++;
	}
	execute_pipeline(pipeline);
	if (main_env && *main_env != pipeline->my_env)
		*main_env = pipeline->my_env;
	return (1);
}
