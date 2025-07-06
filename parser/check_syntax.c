/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:44:34 by francesca         #+#    #+#             */
/*   Updated: 2025/07/06 15:06:58 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

static int	chek_start_token(char **tokens, t_token_type *types, int ntokens)
{
	char	msg[256];

	if (ntokens == 0)
		return (0);
	if (types[0] == PIPE)
		return (exit_shell(2, "syntax error near unexpected token `|'\n"), 0);
	if (types[0] == REDIR_IN || types[0] == REDIR_OUT || types[0] == APPEND
		|| types[0] == HEREDOC)
	{
		snprintf(msg, sizeof(msg), "syntax error near unexpected token `%s'\n",
			tokens[0]);
		exit_shell(2, msg);
		return (0);
	}
	return (1);
}

static int	check_pipe_errors(t_token_type *types, int ntokens)
{
	int	i;

	i = 0;
	while (i < ntokens)
	{
		if (types[i] == PIPE)
		{
			if (i == ntokens - 1)
			{
				exit_shell(2, "syntax error near unexpected token `|'\n");
				return (0);
			}
			if (types[i + 1] == PIPE)
			{
				exit_shell(2, "syntax error near unexpected token `|'\n");
				return (0);
			}
		}
		i++;
	}
	return (1);
}

static int	check_redir(char **tokens, t_token_type *types, int ntokens)
{
	char	msg[256];
	int		i;

	i = 0;
	while (i < ntokens)
	{
		if (types[i] == REDIR_IN || types[i] == REDIR_OUT || types[i] == APPEND
			|| types[i] == HEREDOC)
		{
			if (i == ntokens - 1 || types[i + 1] != WORD)
			{
				snprintf(msg, sizeof(msg),
					"syntax error near unexpected token `%s'\n", tokens[i]);
				exit_shell(2, msg);
				return (0);
			}
		}
		i++;
	}
	return (1);
}

int	check_syntax(char **tokens, t_token_type *types, int ntokens)
{
	if (!chek_start_token(tokens, types, ntokens))
		return (0);
	if (!check_pipe_errors(types, ntokens))
		return (0);
	if (!check_redir(tokens, types, ntokens))
		return (0);
	return (1);
}
