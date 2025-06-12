/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:03:06 by francesca         #+#    #+#             */
/*   Updated: 2025/06/12 14:27:47 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int execute_builtin(t_cmd *cmd, char ***my_envp)
{
    if (!cmd || !cmd->args || !cmd->args[0])
        return (0);
    if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
    {
        if (ft_exit(cmd->args) == 0)
                return (0); // interrompi ciclo
        }
    else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
                ft_pwd();
    else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
            export_myenvp(*my_envp);
    else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
            ft_cd(cmd->args);
    // if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
    //     return (ft_echo(cmd->args));
    // else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
    //     return (ft_cd(cmd->args, envp));
    // else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
    //     return (ft_export(cmd->args, envp));
    // else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
    //     return (ft_unset(cmd->args, envp));
    // else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
    //     return (ft_env(*envp));
    else
        return (1);
    return (1); // ritorna prompt
}
