/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:03:06 by francesca         #+#    #+#             */
/*   Updated: 2025/06/26 09:44:07 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int	execute_builtin(t_cmd *cmd, char ***my_envp, t_pipeline *pipeline)
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
		export_variable(my_envp, cmd->args);
	else if ((ft_strncmp(cmd->args[0], "env", 4) == 0))
		export_myenvp(*my_envp);
	else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		ft_cd(cmd->args, pipeline->my_env);
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0 && cmd->args[1])
		unset_variable(*my_envp, cmd->args);
	else if (ft_strncmp(cmd->args[0], "echo", 4) == 0)
		ft_echo(cmd->args);
	else
		return (1);
	return (1); // ritorna prompt
}
