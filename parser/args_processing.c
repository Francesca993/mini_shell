/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_processing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 15:20:58 by francesca         #+#    #+#             */
/*   Updated: 2025/06/09 15:28:42 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"


void process_args(t_cmd *cmd)
{
    int i = 0;
    
    while (cmd->args && cmd->args[i])
    {
        if (!cmd->quote_single && !cmd->quote_double)
        {
            char *tmp = handle_backslash_outside_quotes(cmd->args[i]);
            free(cmd->args[i]);
            cmd->args[i] = tmp;
        }
        // ...altre gestioni (espansione variabili, ecc.)
        i++;
    }
}

void process_pipeline(t_pipeline *pipeline)
{
    int j = 0;
    
    while (pipeline->cmds[j])
    {
        process_args(pipeline->cmds[j]);
        j++;
    }
}