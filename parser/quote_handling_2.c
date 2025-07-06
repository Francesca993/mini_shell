/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 13:54:46 by francesca         #+#    #+#             */
/*   Updated: 2025/07/06 13:58:12 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

void	find_quotes(t_pipeline *pipeline)
{
	int	i;
	int	j;

	i = 0;
	while (i < pipeline->n_cmds)
	{
		j = 0;
		while (pipeline->cmds[i]->args[j] != NULL)
		{
			if (ft_strchr(pipeline->cmds[i]->args[j], '\''))
				pipeline->cmds[i]->quote_single = 1;
			if (ft_strchr(pipeline->cmds[i]->args[j], '\"'))
				pipeline->cmds[i]->quote_double = 1;
			if (ft_strchr(pipeline->cmds[i]->args[j], '$')
				&& (pipeline->cmds[i]->quote_single == 0))
				pipeline->cmds[i]->dollar = 1;
			j++;
		}
		i++;
	}
	expand_quotes(pipeline);
}
