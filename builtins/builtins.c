/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:03:06 by francesca         #+#    #+#             */
/*   Updated: 2025/06/27 16:07:13 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"
#include <stdio.h>

int is_builtin(t_cmd *cmd)
{
    if (!cmd || !cmd->args || !cmd->args[0])
        return 0;
    if (ft_strcmp(cmd->args[0], "echo") == 0)
        return 1;
    if (ft_strcmp(cmd->args[0], "cd") == 0)
        return 1;
    if (ft_strcmp(cmd->args[0], "pwd") == 0)
        return 1;
    if (ft_strcmp(cmd->args[0], "export") == 0)
        return 1;
    if (ft_strcmp(cmd->args[0], "unset") == 0)
        return 1;
    if (ft_strcmp(cmd->args[0], "env") == 0)
        return 1;
    if (ft_strcmp(cmd->args[0], "exit") == 0)
        return 1;
    return 0;
}

int	execute_builtin(t_cmd *cmd, char ***my_envp, t_pipeline *pipeline)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
	{
		if (ft_exit(cmd->args) == 0)
			return (0); // interrompi ciclo
	}
	if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		return (ft_pwd());
	if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		return (export_variable(my_envp, cmd->args));
	if ((ft_strncmp(cmd->args[0], "env", 4) == 0))
		return (export_myenvp(*my_envp));
	if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		return (ft_cd(cmd->args, pipeline->my_env));
	if (ft_strncmp(cmd->args[0], "unset", 6) == 0 && cmd->args[1])
		return (unset_variable(*my_envp, cmd->args));
	if (ft_strncmp(cmd->args[0], "echo", 4) == 0)
		return (ft_echo(cmd->args));
	else
		return (1);
}
