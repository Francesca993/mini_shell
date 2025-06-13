/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_processing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 15:20:58 by francesca         #+#    #+#             */
/*   Updated: 2025/06/13 12:04:33 by skayed           ###   ########.fr       */
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
        // ...altre gestioni (espansione variabili, ecc.)
        //builtins exit
        i++;
    }
    return (1);
}
int process_pipeline(t_pipeline *pipeline)
{
    int j = 0;
    
    while (pipeline->cmds[j])
    {
        if (process_args(pipeline->cmds[j]) == 0)
            return (0);
        if (execute_builtin(pipeline->cmds[j], &pipeline->my_env) == 0)
            return (0);
        j++;
    }
    return (1);
}