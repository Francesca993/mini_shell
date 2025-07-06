/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 22:23:22 by francesca         #+#    #+#             */
/*   Updated: 2025/07/04 22:25:23 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

void	free_matrix(char **data)
{
	int	i;

	i = 0;
	if (!data)
		return ;
	while (data[i])
		free(data[i++]);
	free(data);
}

void	free_pipeline(t_pipeline *pipeline)
{
	t_cmd	*cmd;
	int		i;

	if (!pipeline)
		return ;
	i = 0;
	while (i < pipeline->n_cmds)
	{
		cmd = pipeline->cmds[i];
		if (!cmd)
			continue ;
		if (cmd->args)
			free_matrix(cmd->args);
		free(cmd->infile);
		free(cmd->outfile);
		free(cmd);
		i++;
	}
	free(pipeline->cmds);
	if (pipeline->tokens)
		free_matrix(pipeline->tokens);
	free(pipeline->types);
	free(pipeline);
}

void	exit_shell(int code, const char *msg)
{
	if (msg && *msg)
	{
		write(STDERR_FILENO, msg, ft_strlen(msg));
	}
	g_exit_status = code;
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
