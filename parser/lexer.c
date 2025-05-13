/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:03:26 by francesca         #+#    #+#             */
/*   Updated: 2025/05/13 12:14:31 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/parser.h"


int lexer(char *line, char ***tokens_out, t_token_type **types_out)
{
    int i = 0;
    int start;
    int len;
    int capacity = INITIAL_CAPACITY;
    int count = 0;
    char **tokens = (char **)ft_calloc(capacity, sizeof(char*));
    t_token_type *types = ft_calloc(capacity, sizeof(t_token_type));

}