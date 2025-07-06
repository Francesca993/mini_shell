/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:58:21 by francesca         #+#    #+#             */
/*   Updated: 2025/07/04 21:48:30 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./header/minishell.h"
#include "header/envp.h"

/*
** Variabile globale usata per memorizzare lo stato di uscita della shell.
** - Aggiornata dopo l'esecuzione di ogni comando (per supportare $?)
** Modificata anche dai signal handler per riflettere l'interruzione 
** da segnali (es. Ctrl-C → 130)
**
** Conforme al subject:
** - È l'unica variabile globale usata per i segnali
** - Non accede né fornisce accesso ad altre strutture dati
** - Di tipo sig_atomic_t per garantire scrittura sicura nei signal handler
*/

volatile sig_atomic_t	g_exit_status = 0;

void	minishell_loop(char ***env)
{
	char		*line;
	t_pipeline	*pipeline;
	int			processing;

	pipeline = NULL;
	processing = 1;
	while (processing == 1)
	{
		line = readline("minishell$: ");
		if (!line)
		{
			exit_shell(0, NULL);
			write(1, "exit\n", 5);
			break ;
		}
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		if (*line)
			add_history(line);
		pipeline = parse_line(line, *env, pipeline);
		if (!pipeline)
		{
			free(line);
			continue ;
		}
		if (pipeline && pipeline->cmds[0] != NULL)
			processing = process_pipeline(pipeline, env);
		if (pipeline)
			free_pipeline(pipeline);
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	**my_env;

	(void)argc;
	(void)argv;
	if (isatty(STDIN_FILENO))
		init_signals();
	my_env = copy_env(envp);
	minishell_loop(&my_env);
	free_myenvp(my_env);
//	rl_clear_history();
	clear_history();
	return (g_exit_status);
}
