/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 10:30:00 by francesca         #+#    #+#             */
/*   Updated: 2025/06/27 16:02:12 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int		export_myenvp(char **my_env)
{
	int	i;

	i = 0;
	while (my_env[i])
	{
		if (ft_strchr(my_env[i], '=')) // stampa solo se contiene '='
			printf("%s\n", my_env[i]);
		i++;
	}
	return (1);
}

void	free_myenvp(char **my_env)
{
	int	i;

	i = 0;
	while (my_env[i])
	{
		free(my_env[i]);
		i++;
	}
	free(my_env);
}

static int	new_env_len(char **envp)
{
	int	len;

	len = 0;
	while (envp[len])
		len++;
	return (len);
}

char	**copy_env(char **envp)
{
	int		i;
	char	**my_env;
	int		len;

	len = new_env_len(envp);
	i = 0;
	my_env = ft_calloc(len + 1, sizeof(char *));
	if (!my_env)
		return (NULL);
	while (i < len)
	{
		my_env[i] = ft_strdup(envp[i]);
		if (!my_env[i])
		{
			while (i-- >= 0)
				free(my_env[i]);
			free(my_env);
			return (NULL);
		}
		i++;
	}
	my_env[i] = NULL;
	return (my_env);
}
