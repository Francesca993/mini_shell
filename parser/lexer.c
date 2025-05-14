/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:03:26 by francesca         #+#    #+#             */
/*   Updated: 2025/05/13 22:32:41 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/parser.h"


int lexer(char *line, char ***tokens_out, t_token_type **types_out)
{
    int n_tokens;
    char **tokens;
    t_token_type *types;
    int filled;
    
    n_tokens = count_token(line);
    tokens = (char**)ft_calloc(n_tokens + 1, sizeof(char *));
    types = ft_calloc(n_tokens, sizeof(t_token_type));
	if (!tokens || !types)
	{
		free(tokens);
		free(types);
		//exit_shell(1, "minishell: malloc failed");
		return -1;
	}
	filled = fill_tokens(line, tokens, types);
	if (filled == -1)
	{
		fprintf(stderr, "syntax error: unclosed quote\n");
		g_exit_status = 2;
		free_partial_tokens(tokens, types, n_tokens);
		return -1;
	}
	*tokens_out = tokens;
	*types_out = types;
	return (filled);
}
