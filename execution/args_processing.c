/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_processing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 15:20:58 by francesca         #+#    #+#             */
/*   Updated: 2025/06/12 12:13:55 by francesca        ###   ########.fr       */
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

// static int test_builtin(char *str)
// {
//     if ((ft_strncmp(str, "echo", 5) == 0) || (ft_strncmp(str, "pwd", 4) == 0) \
//         || (ft_strncmp(str, "cd", 3) == 0) || (ft_strncmp(str, "export", 7) == 0) \
//         || (ft_strncmp(str, "unset", 6) == 0) || (ft_strncmp(str, "env", 4) == 0))
//         {
//             return (1);
//         }
//     return (0);
// }

int process_pipeline(t_pipeline *pipeline)
{
    int j = 0;
    
    while (pipeline->cmds[j])
    {
        if (ft_strncmp(pipeline->cmds[0]->args[0], "exit", 4) == 0)
        {
            if (ft_exit(pipeline->cmds[0]->args) == 0)
                return (0); // interrompi ciclo
        }
        // if (test_builtin(pipeline->cmds[j]->args[0]) == 1)
        // {
        //         execute_builtin(pipeline->cmds[j], pipeline->my_env);
        // }
        if (process_args(pipeline->cmds[j]) == 0)
            return (0);
        j++;
    }
    return (1);
}