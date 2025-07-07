/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 08:21:25 by francesca         #+#    #+#             */
/*   Updated: 2025/07/06 23:04:28 by francesca        ###   ########.fr       */
/*   Updated: 2025/07/07 09:21:48 by skayed           ###   ########.fr       */
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

	* 4. Chiama `populate_comands()` per popolare ogni `t_cmd` all'interno della pipeline.
 *
 * Ritorna:
 * - Puntatore alla `t_pipeline` allocata e popolata.
 * - NULL in caso di errore di allocazione.
 */
t_pipeline	*build_pipeline(char **tokens, t_token_type *types, int num_tokens,
		t_pipeline *pipeline, char **env)
{
	int	num_cmds;

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
static int	count_args_for_cmds(t_token_type *types, int start, int n_tokens)
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

/*
 * Popola l'array di comandi `pipeline->cmds` analizzando token e tipi.
 *
 * Per ogni comando:
 * - Conta i WORD per allocare `cmd->args`.
 * - Alloca e riempie la struttura `t_cmd`:
 *   - `args[]`: solo token di tipo WORD (duplicati con `ft_strdup`).
 *   - Redirezioni:
 *     - <    → `infile`, `redir_in`
 *     - >    → `outfile`, `redir_out`
 *     - >>   → `outfile`, `append`
 *     - <<   → `infile`, `heredoc`
 * - Se è presente una PIPE,
	imposta `cmd->pipe = 1` e passa al prossimo comando.
 *
 * Nota: termina se termina l'array di token.
 */
void	populate_comands(t_pipeline *pipeline)
{
	int		i;
	int		cmd_idx;
	int		n_args;
	t_cmd	*cmd;
	int		arg_idx;
	int		error;

	i = 0;
	cmd_idx = 0;
	while (i < pipeline->n_tokens)
	{
		n_args = count_args_for_cmds(pipeline->types, i, pipeline->n_tokens);
		cmd = ft_calloc(1, sizeof(t_cmd));
		if (!cmd)
			return ;
		cmd->args = ft_calloc(n_args + 1, sizeof(char *));
		if (!cmd->args)
		{
			free(cmd);
			return ;
		}
		cmd->pipeline = pipeline;
		arg_idx = 0;
		while (i < pipeline->n_tokens && pipeline->types[i] != PIPE)
		{
			if (pipeline->types[i] == WORD)
				cmd->args[arg_idx++] = ft_strdup(pipeline->tokens[i]);
			else if (pipeline->types[i] == REDIR_IN && i
					+ 1 < pipeline->n_tokens)
				cmd->infile = ft_strdup(pipeline->tokens[++i]),
				cmd->redir_in = 1;
			else if (pipeline->types[i] == REDIR_OUT && i
					+ 1 < pipeline->n_tokens)
				cmd->outfile = ft_strdup(pipeline->tokens[++i]),
				cmd->redir_out = 1;
			else if (pipeline->types[i] == APPEND && i + 1 < pipeline->n_tokens)
				cmd->outfile = ft_strdup(pipeline->tokens[++i]),
				cmd->append = 1;
			else if (pipeline->types[i] == HEREDOC && i
					+ 1 < pipeline->n_tokens)
				cmd->infile = ft_strdup(pipeline->tokens[++i]),
				cmd->heredoc = 1;
			i++;
		}
		cmd->args[arg_idx] = NULL;
		cmd->fd_in = -1;
		cmd->fd_out = -1;
		error = 0;
		if (cmd->redir_in && cmd->infile)
		{
			if (setup_redir_in(cmd) == -1)
				error = 1;
		}
		if (cmd->redir_out && cmd->outfile)
		{
			if (setup_redir_out(cmd) == -1)
				error = 1;
		}
		if (cmd->append && cmd->outfile)
		{
			if (setup_redir_append(cmd) == -1)
				error = 1;
		}
		if (cmd->heredoc && cmd->infile)
		{
			if (setup_heredoc(cmd, cmd->infile) == -1)
				error = 1;
		}
		if (error)
		{
			free_pipeline(pipeline);
			return ;
		}
		if (i < pipeline->n_tokens && pipeline->types[i] == PIPE)
		{
			cmd->pipe = 1;
			i++;
		}
		pipeline->cmds[cmd_idx++] = cmd;
	}
	find_quotes(pipeline);
	expand_pipeline_variables(pipeline);
}
