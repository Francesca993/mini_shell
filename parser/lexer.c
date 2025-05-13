/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:03:26 by francesca         #+#    #+#             */
/*   Updated: 2025/05/13 14:29:11 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/parser.h"


int lexer(char *line, char ***tokens_out, t_token_type **types_out)
{
    int n_tokens;
    char **tokens;
    t_token_type *types;
    
    n_tokens = count_token(line);
    tokens = (char**)ft_calloc(n_tokens + 1, sizeof(char *));
    types = ft_calloc(n_tokens, sizeof(t_token_type));
    if (!tokens || !types)
        return (-1);
    fill_tokens(line, tokens, types);
    *tokens_out = tokens;
    *types_out = types;
    return (n_tokens);
}