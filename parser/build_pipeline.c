/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 08:21:25 by francesca         #+#    #+#             */
/*   Updated: 2025/07/07 12:12:29 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/execution.h"
#include "../header/minishell.h"

/*
 * Conta quanti comandi sono presenti nella linea, basandosi sulle pipe (|).
 * Ogni pipe separa due comandi, quindi il numero totale è 1 + numero di pipe.
 *
 * Esempio:
 * Input: echo hello | grep test > out.txt
 * Ritorna: 2
 */
static int	count_cmds(char **tokens)
{
	int	count;
	int	i;

	count = 1;
	i = 0;
	while (tokens[i])
	{
		if (ft_strncmp(tokens[i], "|", 2) == 0)
			count++;
		i++;
	}
	return (count);
}

/*
 * Costruisce la struttura `t_pipeline` a partire dai token e dai loro tipi.
 *
 * Passaggi:
 * 1. Conta quanti comandi ci sono (basato sulle pipe).
 * 2. Alloca la struttura `t_pipeline` e l'array di comandi `t_cmd**`.
 * 3. Assegna token, tipi, e dimensioni totali alla pipeline.
 * 4. Chiama `populate_comands()` per popolare ogni `t_cmd` all'interno 
 * della pipeline.
 *
 * Ritorna:
 * - Puntatore alla `t_pipeline` allocata e popolata.
 * - NULL in caso di errore di allocazione.
 */
t_pipeline	*build_pipeline(char **tokens, t_token_type *types, int num_tokens,
		char **env)
{
	t_pipeline	*pipeline;
	int			num_cmds;

	pipeline = ft_calloc(1, sizeof(t_pipeline));
	if (!pipeline)
		return (NULL);
	num_cmds = count_cmds(tokens);
	pipeline->cmds = ft_calloc(num_cmds + 1, sizeof(t_cmd *));
	pipeline->types = types;
	pipeline->tokens = tokens;
	pipeline->n_cmds = num_cmds;
	pipeline->n_tokens = num_tokens;
	pipeline->my_env = env;
	populate_comands(pipeline);
	find_quotes(pipeline);
	expand_pipeline_variables(pipeline);
	return (pipeline);
}

/*
 * Conta quanti token di tipo WORD appartengono a un singolo comando.
 * Serve per allocare correttamente `cmd->args`.
 *
 * Scorre i token a partire da `start` fino a una PIPE o fine input.
 * Salta i token che sono parte di redirezioni (cioè token successivi a <, >,
	>>, <<).
 *
 * Parametri:
 * - types: array di tipi dei token.
 * - start: indice da cui iniziare a contare.
 * - n_tokens: numero totale di token.
 *
 * Ritorna:
 * - Numero di argomenti (WORD) per il comando corrente.
 */
int	count_args_for_cmds(t_token_type *types, int start, int n_tokens)
{
	int	count;
	int	i;

	count = 0;
	i = start;
	while (i < n_tokens && types[i] != PIPE)
	{
		if (types[i] == WORD)
		{
			count++;
		}
		else if ((types[i] == REDIR_IN || types[i] == REDIR_OUT
				|| types[i] == APPEND || types[i] == HEREDOC) && i
			+ 1 < n_tokens)
		{
			i++;
		}
		i++;
	}
	return (count);
}

void	populate_comands(t_pipeline *pipeline)
{
	int		i;
	int		cmd_idx;
	int		n_args;
	t_cmd	*cmd;

	i = 0;
	cmd_idx = 0;
	while (i < pipeline->n_tokens)
	{
		n_args = count_args_for_cmds(pipeline->types, i, pipeline->n_tokens);
		cmd = allocate_cmd(pipeline, n_args);
		if (!cmd)
			return ;
		if (fill_cmds(pipeline, cmd, &i))
			return ;
		if (setup_redirections(cmd))
		{
			free_pipeline(pipeline);
			return ;
		}
		handle_pipe(pipeline, cmd, &i);
		pipeline->cmds[cmd_idx++] = cmd;
	}
	find_quotes(pipeline);
	expand_pipeline_variables(pipeline);
}
