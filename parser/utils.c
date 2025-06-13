/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 22:23:22 by francesca         #+#    #+#             */
/*   Updated: 2025/06/13 12:02:07 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

void	free_pipeline(t_pipeline *pipeline)
{
	t_cmd	*cmd;

	if (!pipeline)
		return ;
	if (pipeline->my_env)
		free_myenvp(pipeline->my_env); // libera my_env direttamente qui
	for (int i = 0; i < pipeline->n_cmds; i++)
	{
		cmd = pipeline->cmds[i];
		if (!cmd)
			continue ;
		if (cmd->args)
		{
			for (int j = 0; cmd->args[j]; j++)
				free(cmd->args[j]);
			free(cmd->args);
		}
		free(cmd->infile);
		free(cmd->outfile);
		free(cmd);
	}
	free(pipeline->cmds);
	if (pipeline->tokens)
	{
		for (int i = 0; i < pipeline->n_tokens; i++)
			free(pipeline->tokens[i]);
		free(pipeline->tokens);
	}
	free(pipeline->types);
	free(pipeline);
}

//libera solo i token allocati fino a count.
void	free_partial_tokens(char **tokens, t_token_type *types, int count)
{
	if (!tokens || !types)
		return ;
	(void)count;
	for (int i = 0; tokens[i]; i++)
		free(tokens[i]);
	free(tokens);
	free(types);
}
void	exit_shell(int code, const char *msg)
{
	if (msg && *msg)
	{
		write(STDERR_FILENO, msg, ft_strlen(msg));
		write(STDERR_FILENO, "\n", 1);
	}
	g_exit_status = code;
}