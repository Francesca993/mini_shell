/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:41:48 by francesca         #+#    #+#             */
/*   Updated: 2025/06/30 08:49:12 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int	ft_pwd(void)
{
	char *cwd;

	// getcwd con NULL → alloca quanto serve
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		exit_shell(1, "pwd");
		return (1);
	}

	// Copia sicura con ft_calloc
	char *safe_copy = ft_calloc(ft_strlen(cwd) + 1, sizeof(char));
	if (!safe_copy)
	{
		free(cwd);
		exit_shell(1, "failed malloc");
		return (1);
	}
	ft_strlcpy(safe_copy, cwd, ft_strlen(cwd) + 1);
	printf("%s\n", safe_copy);
	free(cwd);
	free(safe_copy);
	g_exit_status = 0;
	return (1);
}