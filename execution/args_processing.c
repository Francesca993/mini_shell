/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_processing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 15:20:58 by francesca         #+#    #+#             */
/*   Updated: 2025/06/24 12:09:51 by skayed           ###   ########.fr       */
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
int	process_pipeline(t_pipeline *pipeline, char ***main_env)
{
	int j = 0;

	while (pipeline->cmds[j])
	{
		if (process_args(pipeline->cmds[j]) == 0)
			return (0);
		if (execute_builtin(pipeline->cmds[j], &pipeline->my_env,
				pipeline) == 0)
			return (0);
		else
			execute_pipeline(pipeline);
		j++;
	}
	// Aggiorna l'env del main con le modifiche fatte dalla pipeline
	if (main_env && *main_env != pipeline->my_env)
	{
		*main_env = pipeline->my_env;
	}

	return (1);
}