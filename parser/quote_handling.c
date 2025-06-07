/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 07:43:42 by francesca         #+#    #+#             */
/*   Updated: 2025/06/07 09:15:00 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/quotehandling.h"

char    *remove_single_quote(const char *str)
{
    int     len;
    int     i;
    int     j;
    char    *res;
    
    len = ft_strlen(str);
    res = ft_calloc(len + 1, sizeof(char)); // spazio sufficiente
    i = 0;
    j = 0;

    if (!res)
        return NULL;
    while (str[i])
    {
        if (str[i] != '\'') // rimuovi solo virgolette singole
        {
            res[j++] = str[i];
        }
        i++;
    }
    res[j] = '\0';
    return (res);
}

void    expand_single_quotes(t_cmd *cmd)
{
    int     i;
    char    *new_str;
    
    i = 0;
    while (cmd->args && cmd->args[i])
    {
        if (ft_strchr(cmd->args[i], '\'')) // solo se contiene '
        {
            new_str = remove_single_quote(cmd->args[i]);
            free(cmd->args[i]);
            printf("%s\n", new_str);
            cmd->args[i] = new_str;
        }
        i++;
    }
    
}

/*
* Scorre tutti i comandi della pipeline e se trova le sigle quote lancia expand_single_quote
*che rimuove le quote come la shell in quanto gli errori sono già gestiti prima
*/
void    find_quotes(t_pipeline *pipeline)
{
    int i;

    i = 0;
    while (i < pipeline->n_cmds)
    {
        if (pipeline->cmds[i]->quote_single)
        {
            expand_single_quotes(pipeline->cmds[i]);   // espande come literal
            int j = 0;
            while (pipeline->cmds[i]->args[j])
            {
                printf("debug dopo expande_single quote: %s\n", pipeline->cmds[i]->args[j]);
                j++;
            }
        }

        // if (pipeline->cmds[i]->quote_double)
        //     expand_double_quotes(pipeline->cmds[i]);  // espande variabili, backslash ecc.
        i++;
    }
}