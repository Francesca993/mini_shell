/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 07:43:42 by francesca         #+#    #+#             */
/*   Updated: 2025/06/07 08:19:04 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/quotehandling.h"

void find_quotes(t_pipeline *pipeline)
{
    int i;

    i = 0;
    while (i < pipeline->n_cmds)
    {

    if (pipeline->cmds[i]->quote_single)
        expand_single_quotes(pipeline->cmds[i]);   // espande come literal

    if (pipeline->cmds[i]->quote_double)
        expand_double_quotes(pipeline->cmds[i]);  // espande variabili, backslash ecc.
    
    i++;
    }
}