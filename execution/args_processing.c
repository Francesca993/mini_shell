/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_processing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 15:20:58 by francesca         #+#    #+#             */
/*   Updated: 2025/06/18 17:26:57 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int process_args(t_cmd *cmd)
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
        i++;
    }
    return (1);
}
int process_pipeline(t_pipeline *pipeline, char ***main_env)
{
    int j = 0;
    
    while (pipeline->cmds[j])
    {
        if (process_args(pipeline->cmds[j]) == 0)
            return (0);
        if (execute_builtin(pipeline->cmds[j], &pipeline->my_env, pipeline) == 0)
            return (0);
        j++;
    }
    
    // Aggiorna l'env del main con le modifiche fatte dalla pipeline
    if (main_env && *main_env != pipeline->my_env)
    {
        *main_env = pipeline->my_env;
    }
    
    return (1);
}