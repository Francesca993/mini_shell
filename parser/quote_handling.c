/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 07:43:42 by francesca         #+#    #+#             */
/*   Updated: 2025/06/19 10:30:10 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"
/*
Dal PDF:

Handle ' (single quote): disabilita interpretazione
Handle " (double quote): interpreta solo $
Handle $VAR
Handle $? → ultimo exit status
Quindi in expand_pipeline():

Tracciare quali token sono in quote singole/doppie
Espandere solo se:
Non sono in quote singole
Oppure sono in quote doppie e contengono $
*/

char *strip_outer_quotes(const char *str) {
    size_t len = ft_strlen(str);
    if (len >= 2 && ((str[0] == '\'' && str[len-1] == '\'') || (str[0] == '"' && str[len-1] == '"')))
        return ft_substr(str, 1, len-2);
    return ft_strdup(str);
}

char    *remove_quotes(const char *str) // gestisce sia virgole singole che doppie 
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
        if (str[i] == '\\' && str[i + 1] && (str[i + 1] == '"' || str[i + 1] == '\\'))
        {
            i++;
            res[j++] = str[i];
        }
        else if (str[i] != '\'' && str[i] != '"') 
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
    int i = 0;
    char *new_str;

    while (cmd->args && cmd->args[i])
    {
        // Se l'argomento è racchiuso tra singole quote
        if (cmd->args[i][0] == '\'' && cmd->args[i][ft_strlen(cmd->args[i]) - 1] == '\'')
        {
            new_str = strip_outer_quotes(cmd->args[i]);
            free(cmd->args[i]);
            cmd->args[i] = new_str;
        }
        i++;
    }
}

void expand_double_quotes(t_cmd *cmd, char **env)
{
    int i = 0;
    char *new_str;
    char *expanded;

    while (cmd->args && cmd->args[i])
    {
        if (cmd->args[i][0] == '"' && cmd->args[i][ft_strlen(cmd->args[i]) - 1] == '"')
        {
            new_str = strip_outer_quotes(cmd->args[i]);
            expanded = expand_variables(new_str, env);
            free(new_str);
            free(cmd->args[i]);
            cmd->args[i] = expanded;
        }
        i++;
    }
}

/*
* Scorre tutti i comandi della pipeline e se trova le sigle quote lancia expand_single_quote
*che rimuove le quote come la shell in quanto gli errori sono già gestiti prima
*/
void    expand_quotes(t_pipeline *pipeline)
{
    int i;

    i = 0;
    while (i < pipeline->n_cmds)
    {
        if (pipeline->cmds[i]->quote_single)
        {
            expand_single_quotes(pipeline->cmds[i]);   // espande come literal
        }
        if (pipeline->cmds[i]->quote_double)
        {
            expand_double_quotes(pipeline->cmds[i], pipeline->my_env);
        }
        i++;
    }
}


void find_quotes(t_pipeline *pipeline)
{
    int i = 0;
    int j;
   
    while (i < pipeline->n_cmds)
    {
        j = 0;
        while (pipeline->cmds[i]->args[j] != NULL)
        {
            if (ft_strchr(pipeline->cmds[i]->args[j], '\''))// solo se contiene '
                pipeline->cmds[i]->quote_single = 1;
            if (ft_strchr(pipeline->cmds[i]->args[j], '\"'))
                pipeline->cmds[i]->quote_double = 1;
            if (ft_strchr(pipeline->cmds[i]->args[j], '$') && (pipeline->cmds[i]->quote_single == 0))
                pipeline->cmds[i]->dollar = 1;
            // if (ft_strchr(pipeline->cmds[i]->args[j], '<'))
            //     pipeline->cmds[i]->redir_in = 1;
            // if (ft_strchr(pipeline->cmds[i]->args[j], '>'))
            //     pipeline->cmds[i]->redir_out = 1;
            // if (ft_strchr(pipeline->cmds[i]->args[j], '>>'))
            //     pipeline->cmds[i]->append = 1;
            // if (ft_strchr(pipeline->cmds[i]->args[j], '<<'))
            //     pipeline->cmds[i]->heredoc = 1;
            j++;
        }
        i++;
    }
    expand_quotes(pipeline);
    
}
