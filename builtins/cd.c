/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 14:03:41 by francesca         #+#    #+#             */
/*   Updated: 2025/06/20 17:15:49 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

void	update_env_value(char **my_env, char *new_path)
{
	int	i;
	int	flag;

	char *temp_path = ft_strjoin("PWD=", new_path); // "PWD=/path"
	flag = 0;
	i = 0;
	// if (!new_path)
	//     return ;
	while (my_env[i])
	{
		if (ft_strncmp(my_env[i], "PWD", 3) == 0 && my_env[i][3] == '=')
		{
			flag = 1;
			free(my_env[i]);
			my_env[i] = temp_path;
			return ;
		}
		i++;
	}
	if (flag == 0)
		free(new_path);
}

/*
Obiettivo: cd [path]
Solo un argomento (cd path)
Nessuna opzione richiesta
Deve modificare la working directory del processo
Deve gestire errori: path mancante, path non esistente, troppi argomenti
📦 Comportamenti richiesti
Comando	Azione
cd /tmp	Cambia directory a /tmp
cd	Bash va in $HOME (non richiesto dal subject)
cd pippo pluto	Errore: too many arguments
cd nonesiste	Errore: no such file or directory
*/
int	ft_cd(char **args, char **my_env)
{
	char	*path;

	if (!args[1])
	{
		path = getenv("HOME");
		if (!path)
		{
			exit_shell(1, "minishell: cd: HOME not set\n");
			return (1);
		}
		// exit_shell(1, "minishell: cd: missing argument\n");
		// return (1);
	}
	else if (args[2])
	{
		exit_shell(1, "minishell: cd: too many arguments\n");
		return (1);
	}
	else
		path = args[1]; // path = argomento dell'utente
	if (chdir(path) != 0)
	{
		exit_shell(1, "minishell: cd: no such file or directory, ");
		perror("");
		return (1);
	}
	else
	{
		path = getcwd(NULL, 0);
		update_env_value(my_env, path);
	}
	g_exit_status = 0;
	return (1);
}
