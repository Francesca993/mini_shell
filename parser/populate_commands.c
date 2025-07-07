/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   populate_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:18:25 by francesca         #+#    #+#             */
/*   Updated: 2025/07/07 12:15:49 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/execution.h"
#include "../header/minishell.h"

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
// void	populate_comands(t_pipeline *pipeline)
// {
// 	int		i;
// 	int		cmd_idx;
// 	int		n_args;
// 	t_cmd	*cmd;
// 	int		arg_idx;
// 	int		error;

// 	i = 0;
// 	cmd_idx = 0;
// 	while (i < pipeline->n_tokens)
// 	{
// 		n_args = count_args_for_cmds(pipeline->types, i, pipeline->n_tokens);
// 		cmd = ft_calloc(1, sizeof(t_cmd));
// 		if (!cmd)
// 			return ;
// 		cmd->args = ft_calloc(n_args + 1, sizeof(char *));
// 		if (!cmd->args)
// 		{
// 			free(cmd);
// 			return ;
// 		}
// 		cmd->pipeline = pipeline;
// 		arg_idx = 0;
// 		while (i < pipeline->n_tokens && pipeline->types[i] != PIPE)
// 		{
// 			if (pipeline->types[i] == WORD)
// 				cmd->args[arg_idx++] = ft_strdup(pipeline->tokens[i]);
// 			else if (pipeline->types[i] == REDIR_IN && i
// 					+ 1 < pipeline->n_tokens)
// 				cmd->infile = ft_strdup(pipeline->tokens[++i]),
// 				cmd->redir_in = 1;
// 			else if (pipeline->types[i] == REDIR_OUT && i
// 					+ 1 < pipeline->n_tokens)
// 				cmd->outfile = ft_strdup(pipeline->tokens[++i]),
// 				cmd->redir_out = 1;
// 			else if (pipeline->types[i] == APPEND && i + 1 < pipeline->n_tokens)
// 				cmd->outfile = ft_strdup(pipeline->tokens[++i]),
// 				cmd->append = 1;
// 			else if (pipeline->types[i] == HEREDOC && i
// 					+ 1 < pipeline->n_tokens)
// 				cmd->infile = ft_strdup(pipeline->tokens[++i]),
// 				cmd->heredoc = 1;
// 			i++;
// 		}
// 		cmd->args[arg_idx] = NULL;
// 		cmd->fd_in = -1;
// 		cmd->fd_out = -1;
// 		error = 0;
// 		if (cmd->redir_in && cmd->infile)
// 		{
// 			if (setup_redir_in(cmd) == -1)
// 				error = 1;
// 		}
// 		if (cmd->redir_out && cmd->outfile)
// 		{
// 			if (setup_redir_out(cmd) == -1)
// 				error = 1;
// 		}
// 		if (cmd->append && cmd->outfile)
// 		{
// 			if (setup_redir_append(cmd) == -1)
// 				error = 1;
// 		}
// 		if (cmd->heredoc && cmd->infile)
// 		{
// 			if (setup_heredoc(cmd, cmd->infile) == -1)
// 				error = 1;
// 		}
// 		if (error)
// 		{
// 			free_pipeline(pipeline);
// 			return ;
// 		}
// 		if (i < pipeline->n_tokens && pipeline->types[i] == PIPE)
// 		{
// 			cmd->pipe = 1;
// 			i++;
// 		}
// 		pipeline->cmds[cmd_idx++] = cmd;
// 	}
// 	find_quotes(pipeline);
// 	expand_pipeline_variables(pipeline);
// }

t_cmd	*allocate_cmd(t_pipeline *pipeline, int n_args)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = ft_calloc(n_args + 1, sizeof(char *));
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	cmd->pipeline = pipeline;
	cmd->fd_in = -1;
	cmd->fd_out = -1;
	return (cmd);
}

void	handle_command_redirection(t_pipeline *p, t_cmd *cmd, int *i)
{
	t_token_type	type;

	type = p->types[*i];
	if ((*i + 1) >= p->n_tokens)
		return ;
	if (type == REDIR_IN)
	{
		cmd->infile = ft_strdup(p->tokens[++(*i)]);
		cmd->redir_in = 1;
	}
	else if (type == REDIR_OUT)
	{
		cmd->outfile = ft_strdup(p->tokens[++(*i)]);
		cmd->redir_out = 1;
	}
	else if (type == APPEND)
	{
		cmd->outfile = ft_strdup(p->tokens[++(*i)]);
		cmd->append = 1;
	}
	else if (type == HEREDOC)
	{
		cmd->infile = ft_strdup(p->tokens[++(*i)]);
		cmd->heredoc = 1;
	}
}

int	fill_cmds(t_pipeline *pipeline, t_cmd *cmd, int *i)
{
	int	arg_idx;

	arg_idx = 0;
	while (*i < pipeline->n_tokens && pipeline->types[*i] != PIPE)
	{
		if (pipeline->types[*i] == WORD)
			cmd->args[arg_idx++] = ft_strdup(pipeline->tokens[*i]);
		else
			handle_command_redirection(pipeline, cmd, i);
		(*i)++;
	}
	cmd->args[arg_idx] = NULL;
	return (0);
}

int	setup_redirections(t_cmd *cmd)
{
	if (cmd->redir_in && cmd->infile && setup_redir_in(cmd) == -1)
		return (1);
	if (cmd->redir_out && cmd->outfile && setup_redir_out(cmd) == -1)
		return (1);
	if (cmd->append && cmd->outfile && setup_redir_append(cmd) == -1)
		return (1);
	if (cmd->heredoc && cmd->infile && setup_heredoc(cmd, cmd->infile) == -1)
		return (1);
	return (0);
}

void	handle_pipe(t_pipeline *pipeline, t_cmd *cmd, int *i)
{
	if (*i < pipeline->n_tokens && pipeline->types[*i] == PIPE)
	{
		cmd->pipe = 1;
		(*i)++;
	}
}
